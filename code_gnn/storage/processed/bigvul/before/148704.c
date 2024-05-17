V4L2JpegEncodeAccelerator::EncodedInstance::EncodedInstance(
    V4L2JpegEncodeAccelerator* parent)
    : parent_(parent),
      input_streamon_(false),
      output_streamon_(false),
      input_buffer_pixelformat_(0),
      input_buffer_num_planes_(0),
      output_buffer_pixelformat_(0) {}
