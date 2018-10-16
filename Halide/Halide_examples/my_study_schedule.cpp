#include <stdio.h>
#include "Halide.h"

using namespace Halide;

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
#define NUM_ORIG_ELEM 6
	int ref = 2;
	Var x("x"), y("y");
	Func orig("orig");
	orig(x) = x;

	Func reordered("reordered");
	reordered(x) = select(x < ref, orig(x), orig(x + 1));

	reordered.bound(x, 0, NUM_ORIG_ELEM-1).unroll(x);
	Buffer<int> result = reordered.realize(NUM_ORIG_ELEM - 1);
	for (int i = 0; i < result.width(); i++)
		std::cout << ' ' << result(i);
	std::cout << std::endl;

	reordered.print_loop_nest();
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