  FormatConverter(const IntRect& source_data_sub_rectangle,
                  int depth,
                  int unpack_image_height,
                  const void* src_start,
                  void* dst_start,
                  int src_stride,
                  int src_row_offset,
                  int dst_stride)
      : src_sub_rectangle_(source_data_sub_rectangle),
        depth_(depth),
        unpack_image_height_(unpack_image_height),
        src_start_(src_start),
        dst_start_(dst_start),
        src_stride_(src_stride),
        src_row_offset_(src_row_offset),
        dst_stride_(dst_stride),
        success_(false) {
    const unsigned kMaxNumberOfComponents = 4;
    const unsigned kMaxBytesPerComponent = 4;
    unpacked_intermediate_src_data_ = WrapArrayUnique(
        new uint8_t[src_sub_rectangle_.Width() * kMaxNumberOfComponents *
                    kMaxBytesPerComponent]);
    DCHECK(unpacked_intermediate_src_data_.get());
  }
