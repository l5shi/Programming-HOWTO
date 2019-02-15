#include "Halide.h"
#include <stdio.h>

// from https://github.com/halide/Halide/blob/master/test/auto_schedule/histogram.cpp
#include "halide_benchmark.h"

using namespace Halide;
using namespace Halide::Tools;

#define NUM_THREADS 4
double run_test(bool auto_schedule) {
	//int W = 1920;
	//int H = 1024;
    int W = 4000;
    int H = 3000;
	Buffer<uint8_t> in(W, H, 3);

	for (int y = 0; y < in.height(); y++) {
		for (int x = 0; x < in.width(); x++) {
			for (int c = 0; c < 3; c++) {
				in(x, y, c) = rand() & 0xfff;
			}
		}
	}

	Var x("x"), y("y"), c("c");

	Func Y("Y");
	Y(x, y) = 0.299f * in(x, y, 0) + 0.587f * in(x, y, 1) + 0.114f * in(x, y, 2);

	Func Cr("Cr");
	Expr R = in(x, y, 0);
	Cr(x, y) = (R - Y(x, y)) * 0.713f + 128;

	Func Cb("Cb");
	Expr B = in(x, y, 2);
	Cb(x, y) = (B - Y(x, y)) * 0.564f + 128;

	Func hist_rows("hist_rows");
	hist_rows(x, y) = 0;
	RDom rx(0, in.width());
	Expr bin = cast<uint8_t>(clamp(Y(rx, y), 0, 255));
	hist_rows(bin, y) += 1;

	Func hist("hist");
	hist(x) = 0;
	RDom ry(0, in.height());
	hist(x) += hist_rows(x, ry);

	Func cdf("cdf");
	cdf(x) = hist(0);
	RDom b(1, 255);
	cdf(b.x) = cdf(b.x - 1) + hist(b.x);

	Func eq("equalize");

	Expr cdf_bin = cast<uint8_t>(clamp(Y(x, y), 0, 255));
	eq(x, y) = clamp(cdf(cdf_bin) * (255.0f / (in.height() * in.width())), 0, 255);

	Func color("color");
	Expr red = cast<uint8_t>(clamp(eq(x, y) + (Cr(x, y) - 128) * 1.4f, 0, 255));
	Expr green = cast<uint8_t>(clamp(eq(x, y) - 0.343f * (Cb(x, y) - 128) - 0.711f * (Cr(x, y) - 128), 0, 255));
	Expr blue = cast<uint8_t>(clamp(eq(x, y) + 1.765f * (Cb(x, y) - 128), 0, 255));
	color(x, y, c) = select(c == 0, red, select(c == 1, green, blue));

	Target target = get_jit_target_from_environment();
	Pipeline p(color);

	if (auto_schedule) {
		// Provide estimates on the pipeline output
		color.estimate(x, 0, 1920).estimate(y, 0, 1024).estimate(c, 0, 3);
		// Auto-schedule the pipeline
		p.auto_schedule(target);
	}
	else if (target.has_gpu_feature()) {
		Var xi("xi"), yi("yi");
		Y.compute_root().gpu_tile(x, y, xi, yi, 16, 16);
		hist_rows.compute_root().gpu_tile(y, yi, 16).update().gpu_tile(y, yi, 16);
		hist.compute_root().gpu_tile(x, xi, 16).update().gpu_tile(x, xi, 16);
		cdf.compute_root().gpu_single_thread();
		Cr.compute_at(color, xi);
		Cb.compute_at(color, xi);
		eq.compute_at(color, xi);
		color.compute_root()
			.reorder(c, x, y).bound(c, 0, 3).unroll(c)
			.gpu_tile(x, y, xi, yi, 16, 16);
	}
	else {
		Y.compute_root().parallel(y, NUM_THREADS).vectorize(x, 8);

		hist_rows.compute_root()
			.vectorize(x, 8)
			.parallel(y, NUM_THREADS)
			.update()
			.parallel(y, NUM_THREADS);
		hist.compute_root()
			.vectorize(x, 8)
			.update()
			.reorder(x, ry)
			.vectorize(x, 8)
			.unroll(x, 4)
			.parallel(x)
			.reorder(ry, x);

		cdf.compute_root();
		eq.compute_at(color, x).unroll(x);
		Cb.compute_at(color, x).vectorize(x);
		Cr.compute_at(color, x).vectorize(x);
		color.reorder(c, x, y)
			.bound(c, 0, 3)
			.unroll(c)
			.parallel(y, NUM_THREADS)
			.vectorize(x, 8);
	}

	p.compile_to_lowered_stmt("histogram.html", { in }, HTML, target);
	color.print_loop_nest();

	Buffer<uint8_t> out(in.width(), in.height(), in.channels());
	double t = benchmark(3, 10, [&]() {
		p.realize(out);
	});

	return t * 1000;
}

int main(int argc, char **argv) {
	double manual_time = run_test(false);
	//double auto_time = run_test(true);

	std::cout << "======================" << std::endl;
	std::cout << "Manual time: " << manual_time << "ms" << std::endl;
	//std::cout << "Auto time: " << auto_time << "ms" << std::endl;
	//std::cout << "======================" << std::endl;

	//if (auto_time > manual_time * 3) {
	//	printf("Auto-scheduler is much much slower than it should be.\n");
	//	return -1;
	//}

	printf("Success!\n");
	return 0;
}

#if 0  // define_extern
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

// More Halide extern function example
//  https://github.com/halide/Halide/blob/master/test/correctness/skip_stages_external_array_functions.cpp
// https://github.com/halide/Halide/blob/master/test/generator/define_extern_opencl_generator.cpp
// https://github.com/halide/Halide/blob/master/test/generator/define_extern_opencl_aottest.cpp#L119
extern "C" DLLEXPORT int flip_x(halide_buffer_t *in1, halide_buffer_t *in2, halide_buffer_t *out) {
	int min = out->dim[0].min;
	int max = out->dim[0].min + out->dim[0].extent - 1;

	int extent = out->dim[0].extent;
	int flipped_min = -max;
	int flipped_max = -min;

	if (in1->host == nullptr || in2->host == nullptr) {
		// If any of the inputs have a null host pointer, we're in
		// bounds inference mode, and should mutate those input
		// buffers that have a null host pointer.
		printf("Doing flip_x bounds inference over [%d %d]\n", min, max);
		if (in1->is_bounds_query()) {
			printf("\tbounds inference for in1\n");
			in1->dim[0].min = flipped_min;
			in1->dim[0].extent = extent;
		}
		if (in2->is_bounds_query()) {
			printf("\tbounds inference for in2\n");
			in2->dim[0].min = flipped_min;
			in2->dim[0].extent = extent;
		}
		// We don't mutate the output buffer, because we can handle
		// any size output.

		//printf("Bounds inference flip_x over [%d %d] requires [%d %d]\n", min, extent, flipped_min, extent);
	}
	else {
		assert(in1->type == halide_type_of<uint8_t>());
		assert(in2->type == halide_type_of<int32_t>());
		assert(out->type == halide_type_of<uint8_t>());

		printf("Computing flip_x over [%d %d]: in buffer 0x%llx\n", min, max, (uint64_t)in1->host);

		// Check the inputs are as large as we expected. They should
		// be, if the above bounds inference code is right.
		assert(in1->dim[0].min <= flipped_min &&
			in1->dim[0].min + in1->dim[0].extent > flipped_max);
		assert(in2->dim[0].min <= flipped_min &&
			in2->dim[0].min + in2->dim[0].extent > flipped_max);

		// Check the strides are what we want.
		assert(in1->dim[0].stride == 1 && in2->dim[0].stride == 1 && out->dim[0].stride == 1);

		// Get pointers to the origin from each of the inputs (because
		// we're flipping about the origin)
		uint8_t *dst = (uint8_t *)(out->host) - out->dim[0].min;
		uint8_t *src1 = (uint8_t *)(in1->host) - in1->dim[0].min;
		int *src2 = (int *)(in2->host) - in2->dim[0].min;

		// Do the flip.
		for (int i = min; i <= max; i++) {
			dst[i] = src1[-i] + src2[-i];
		}
	}

	return 0;
}

using namespace Halide;

int main(int argc, char **argv) {
	Func f("f"), g("flip_x"), h("h");
	Var x("x");

	// Make some input data in the range [-99, 0]
	Buffer<uint8_t> input(100);
	printf("input buffer 0x%llx\n", (uint64_t)input.data());
	input.set_min(-99);
	lambda(x, cast<uint8_t>(x*x)).realize(input);

	assert(input(-99) == (uint8_t)(-99 * -99));

	f(x) = x * x;

	std::vector<ExternFuncArgument> args(2);
	args[0] = input;
	args[1] = f;
	g.define_extern("flip_x", args, UInt(8), 1);

	h(x) = g(x) * 2;

	f.compute_at(h, x);
	g.compute_at(h, x);
	Var xi("xi");
	h.vectorize(x, 8).unroll(x, 2).split(x, x, xi, 4).parallel(x);

	Buffer<uint8_t> result = h.realize(100);
	h.print_loop_nest();

	for (int i = 0; i < 100; i++) {
		uint8_t correct = 4 * i*i;
		if (result(i) != correct) {
			printf("result(%d) = %d instead of %d\n", i, result(i), correct);
			return -1;
		}
	}

	printf("Success!\n");
	return 0;
}
#endif

#if 0
#include <stdio.h>
#include "Halide.h"

using namespace Halide;
using namespace std;

#define TILE_SIZE  8
#define NUM_IMGS   10

Func TransformImg( Func imgs_tiled )
{
	Var c("c"), n("n");
	Var ix("ix"), iy("iy"), tx("tx"), ty("ty");

	Func f("TransformedImg");
	f(ix, iy, tx, ty, c, n) = Halide::sqrt(Halide::abs(imgs_tiled(ix, iy, tx, ty, c, n)));

	return f;
}

int main(int argc, char **argv)
{
	Func f;
	cout << "sizeof(Func): " << sizeof(Func) << " " << sizeof(f) << endl;
	FuncRef fr;
	cout << "sizeof(Func): " << sizeof(Func) << " " << sizeof(f) << endl;

	Buffer<uint8_t> buf;
	Buffer<uint8_t> img(100,200);
	cout << "sizeof Buffer<uint8_t> " << sizeof(buf) << endl;
	cout << "sizeof Buffer<uint8_t> img(100,200) " << sizeof(img) << endl;



	//// example from: https://stackoverflow.com/questions/43168885/is-there-any-way-to-combine-funcs-into-a-func-has-one-more-dimension
	Var x("x"), y("y");
	Func f("f"), g("g");
	RDom range(0, 3, 0, 3, "range"); // Form is min/extent, not start/end

	//f(x, y) = 0; // Initial condition
	//f(range.x, range.y) = f(range.x - 1, range.y - 1) + 1;
	f(x, y) = x + y;
	//f.trace_stores();

	//g(x, y) = f(x, y);
	cout << g.name() << endl;
	g = f;
	//g(x, y) = x+y;
	cout << g.name() << endl;

	Buffer<int32_t> result = g.realize(3, 3);

	g.print_loop_nest();
	//for (int r = 0; r < 3; r++) {
	//	for (int c = 0; c < 3; c++)
	//		cout << result(c, r) << ' ';
	//	cout << endl;
	//}
#if 0
#define NUM_ORIG_ELEM 6
	int ref = 2;
	Var x("x"), y("y");
	Func orig("orig");
	orig(x) = x;

	Func reordered("reordered");
	reordered(x) = select(x < ref, orig(x), orig(x + 1));

	//reordered.bound(x, 0, NUM_ORIG_ELEM-1).unroll(x);
	//reordered.bound(x, 0, 10).unroll(x);
	reordered.unroll(x);
	Buffer<int> result = reordered.realize(NUM_ORIG_ELEM - 1);
	for (int i = 0; i < result.width(); i++)
		std::cout << ' ' << result(i);
	std::cout << std::endl;

	reordered.print_loop_nest();
#endif
#if 0
	// Attempt 2:
	Var x("x"), y("y");
	Func producer("producer");
	producer(x, y) = (x * y) / 10 + 8;

	Func producer_1("producer_1"), producer_2("producer_2"), consumer_2("consumer_2");
	//producer_1(x, y) = producer(x, y);
	producer_2(x, y) = producer(x, y);

	consumer_2(x, y) = x + y;
	consumer_2(x, 0) += producer(x, x);
	consumer_2(0, y) += producer_2(y, 9 - y);

	// The wrapper functions give us two separate handles on
	// the producer, so we can schedule them differently.
	producer.compute_at(consumer_2, x);
	producer_2.compute_at(consumer_2, y);

	Buffer<int> halide_result = consumer_2.realize(10, 10);

	consumer_2.print_loop_nest();
#endif

#if 0
	Var x("x"), y("y"), c("c"), n("n");
	Func imgs("InputImg");
	imgs(x, y, c, n) = n * 10 + c;

	Var ix("ix"), iy("iy"), tx("tx"), ty("ty");
	Func imgs_tiled("TiledImgs");
	imgs_tiled(ix, iy, tx, ty, c, n) = imgs( tx * TILE_SIZE + ix, ty * TILE_SIZE + iy, c, n);

	Func imgs_transformed("imgs_transformed");
	imgs_transformed = TransformImg( imgs_tiled );
	//imgs_transformed(ix, iy, tx, ty, c, n) = Halide::sqrt(Halide::abs(imgs_tiled(ix, iy, tx, ty, c, n)));
	Func imgs_transformed_wrap("TransformedImg_Wrap");
	imgs_transformed_wrap(ix, iy, tx, ty, c, n) = imgs_transformed(ix, iy, tx, ty, c, n);

	Func merged("MergedImg");
	RDom r_n(1, NUM_IMGS-1, "R_Imgs");

	//Expr d = imgs_transformed(ix, iy, tx, ty, c, r_n) - imgs_transformed(ix, iy, tx, ty, c, 0);
	//Func alt_img_sum("AltImsgSum");
	//alt_img_sum(ix, iy, tx, ty, c) = sum(d * d);
	//merged(ix, iy, tx, ty, c) = imgs_transformed(ix, iy, tx, ty, c, 0) + alt_img_sum(ix, iy, tx, ty, c);

	RDom r(1, NUM_IMGS-1, "R_imgs");
	merged(ix, iy, tx, ty, c) = imgs_transformed(ix, iy, tx, ty, c, 0);
	merged(ix, iy, tx, ty, c) += imgs_transformed_wrap(ix, iy, tx, ty, c, r);

	Func merged_final("FinalMerged");
	merged_final(ix, iy, tx, ty, c) = merged(ix, iy, tx, ty, c);
	//merged(ix, iy, tx, ty, c) = imgs_transformed(ix, iy, tx, ty, c, 0) + imgs_transformed(ix, iy, tx, ty, c, 1);
	//imgs_tiled.compute_root();

	// schedule
	//imgs_tiled.compute_root();
	//imgs_transformed.compute_root( );
	//merged.reorder(ix, iy, c, tx, ty);
	//imgs_transformed.compute_at( merged, c );
	imgs_transformed.reorder(ix, iy, c, n, tx, ty); 
	imgs_transformed.compute_at(merged, tx);
	imgs_transformed_wrap.reorder(ix, iy, c, n,	 tx, ty);
	imgs_transformed_wrap.compute_at(merged, r);
	merged.reorder(ix, iy, c, tx, ty);
	merged.update(0).reorder(ix, iy, c, r, tx, ty);

	merged.compute_at(merged_final, tx);
	merged_final.reorder(ix, iy, c, tx, ty);
	merged_final.realize(std::vector<int>{16, 16, 64, 64, 4});

	//merged.print_loop_nest();
	merged_final.print_loop_nest();

	//Func data("Data");
	//data(c, n) = c + n * 10;

	//Func accum("Accum");
	//RDom rduc(0, 10, "Rduc");
	//accum(c) = 0;
	//accum(c) += data(c, rduc);

	//accum.update(0).reorder(c, rduc);
	//accum.realize(4);
	//accum.print_loop_nest();
#endif
	return 0;
}

#endif