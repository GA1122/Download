bool WebGLImageConversion::PackPixels(const uint8_t* source_data,
                                      DataFormat source_data_format,
                                      unsigned source_data_width,
                                      unsigned source_data_height,
                                      const IntRect& source_data_sub_rectangle,
                                      int depth,
                                      unsigned source_unpack_alignment,
                                      int unpack_image_height,
                                      unsigned destination_format,
                                      unsigned destination_type,
                                      AlphaOp alpha_op,
                                      void* destination_data,
                                      bool flip_y) {
  DCHECK_GE(depth, 1);
  if (unpack_image_height == 0) {
    unpack_image_height = source_data_sub_rectangle.Height();
  }
  int valid_src = source_data_width * TexelBytesForFormat(source_data_format);
  int remainder =
      source_unpack_alignment ? (valid_src % source_unpack_alignment) : 0;
  int src_stride =
      remainder ? (valid_src + source_unpack_alignment - remainder) : valid_src;
  int src_row_offset =
      source_data_sub_rectangle.X() * TexelBytesForFormat(source_data_format);

  DataFormat dst_data_format =
      GetDataFormat(destination_format, destination_type);
  if (dst_data_format == kDataFormatNumFormats)
    return false;
  int dst_stride =
      source_data_sub_rectangle.Width() * TexelBytesForFormat(dst_data_format);
  if (flip_y) {
    destination_data =
        static_cast<uint8_t*>(destination_data) +
        dst_stride * ((depth * source_data_sub_rectangle.Height()) - 1);
    dst_stride = -dst_stride;
  }
  if (!HasAlpha(source_data_format) || !HasColor(source_data_format) ||
      !HasColor(dst_data_format))
    alpha_op = kAlphaDoNothing;

  if (source_data_format == dst_data_format && alpha_op == kAlphaDoNothing) {
    const uint8_t* base_ptr =
        source_data + src_stride * source_data_sub_rectangle.Y();
    const uint8_t* base_end =
        source_data + src_stride * source_data_sub_rectangle.MaxY();

    if (flip_y && depth > 1) {
      const ptrdiff_t distance_to_top_image =
          (depth - 1) * src_stride * unpack_image_height;
      base_ptr -= distance_to_top_image;
      base_end -= distance_to_top_image;
    }

    unsigned row_size = (dst_stride > 0) ? dst_stride : -dst_stride;
    uint8_t* dst = static_cast<uint8_t*>(destination_data);

    for (int i = 0; i < depth; ++i) {
      const uint8_t* ptr = base_ptr;
      const uint8_t* ptr_end = base_end;
      while (ptr < ptr_end) {
        memcpy(dst, ptr + src_row_offset, row_size);
        ptr += src_stride;
        dst += dst_stride;
      }
      base_ptr += unpack_image_height * src_stride;
      base_end += unpack_image_height * src_stride;
    }
    return true;
  }

  FormatConverter converter(source_data_sub_rectangle, depth,
                            unpack_image_height, source_data, destination_data,
                            src_stride, src_row_offset, dst_stride);
  converter.Convert(source_data_format, dst_data_format, alpha_op);
  if (!converter.Success())
    return false;
  return true;
}
