<!-- TOC -->

- [Concepts](#concepts)
    - [Datum](#datum)
- [Util](#util)
    - [`./src/gtest/gtest.h`](#srcgtestgtesth)
- [ImageNetData Preparation Example](#imagenetdata-preparation-example)
    - [`examples/imagenet/create_imagenet.sh`](#examplesimagenetcreate_imagenetsh)
        - [`tools/convert_imageset.cpp`](#toolsconvert_imagesetcpp)
            - [`src/caffe/util/io.cpp`](#srccaffeutiliocpp)

<!-- /TOC -->

## Concepts

### Datum

The Datum object is defined with **protobuf** in [`src/caffe/proto/caffe.proto`](https://github.com/BVLC/caffe/blob/master/src/caffe/proto/caffe.proto#L30-L41). The protocol buffer compiler generates a file `caffe.pb.h` in `.build_release/src/caffe/proto` with the class `Datum`. 
```c++
message Datum {
  optional int32 channels = 1;
  optional int32 height = 2;
  optional int32 width = 3;
  // the actual image data, in bytes
  optional bytes data = 4;
  optional int32 label = 5;
  // Optionally, the datum could also hold float data.
  repeated float float_data = 6;
  // If true data contains an encoded image that need to be decoded
  optional bool encoded = 7 [default = false];
}
```

Datum order is different from OpenCV Mat order: a channel is stored continuously
`int datum_index = (c * datum_height + h) * datum_width + w;`

```c++
datum->set_channels(cv_img.channels());
datum->set_height(cv_img.rows);
datum->set_width(cv_img.cols);
datum->set_encoded(false);
int datum_channels = datum->channels();
int datum_height = datum->height();
int datum_width = datum->width();
int datum_size = datum_channels * datum_height * datum_width;
//...
datum->set_data(buffer);
datum->set_label(label);
```

## Util

### `./src/gtest/gtest.h`

```c++
#define GTEST_FLAG(name) FLAGS_gtest_##name

#define GTEST_DEFINE_string_(name, default_val, doc) \
    GTEST_API_ ::testing::internal::String GTEST_FLAG(name) = (default_val)
```

## ImageNetData Preparation Example

**Convert images to DB**

### `examples/imagenet/create_imagenet.sh`
```sh
# Default: not encoded
convert_imageset \
    --resize_height=$RESIZE_HEIGHT \
    --resize_width=$RESIZE_WIDTH \
    --shuffle \
    $TRAIN_DATA_ROOT \
    $DATA/train.txt \ // a file listing the images to be added to the DB
    $EXAMPLE/ilsvrc12_train_lmdb
```

#### `tools/convert_imageset.cpp`

```c++
// Read image file list and store it into vector of (fileName, label) pairs
std::vector<std::pair<std::string, int> > lines;
while(...):
    lines.push_back(std::make_pair(line.substr(0, pos), label));

for (int line_id = 0; line_id < lines.size(); ++line_id) {
    //...
    status = ReadImageToDatum(root_folder + lines[line_id].first, // file name
        lines[line_id].second, // label
        resize_height, resize_width, is_color,
        enc, &datum);

    // Put in db
    string out;
    CHECK(datum.SerializeToString(&out)); //datum to out
    txn->Put(key_str, out);
}
```

##### `src/caffe/util/io.cpp`

```c++
bool ReadImageToDatum(const string& filename, const int label,
    const int height, const int width, const bool is_color,
    const std::string & encoding, Datum* datum) {
  cv::Mat cv_img = ReadImageToCVMat(filename, height, width, is_color);
  if (cv_img.data) {
    if (encoding.size()) { // default: not encoded
      if ( (cv_img.channels() == 3) == is_color && !height && !width &&
          matchExt(filename, encoding) )
        return ReadFileToDatum(filename, label, datum);
      std::vector<uchar> buf;
      cv::imencode("."+encoding, cv_img, buf);
      datum->set_data(std::string(reinterpret_cast<char*>(&buf[0]),
                      buf.size()));
      datum->set_label(label);
      datum->set_encoded(true);
      return true;
    }
    CVMatToDatum(cv_img, datum);
    datum->set_label(label);
    return true;
  } else {
    return false;
  }
}
```
