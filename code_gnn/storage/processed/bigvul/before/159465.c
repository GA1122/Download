bool WebGLImageConversion::ExtractImageData(
    const uint8_t* image_data,
    DataFormat source_data_format,
    const IntSize& image_data_size,
    const IntRect& source_image_sub_rectangle,
    int depth,
    int unpack_image_height,
    GLenum format,
    GLenum type,
    bool flip_y,
    bool premultiply_alpha,
    Vector<uint8_t>& data) {
  if (!image_data)
    return false;
  int width = image_data_size.Width();
  int height = image_data_size.Height();

  unsigned packed_size;
  PixelStoreParams params;
  params.alignment = 1;
  if (ComputeImageSizeInBytes(format, type, source_image_sub_rectangle.Width(),
                              source_image_sub_rectangle.Height(), depth,
                              params, &packed_size, nullptr,
                              nullptr) != GL_NO_ERROR)
    return false;
  data.resize(packed_size);

  if (!PackPixels(image_data, source_data_format, width, height,
                  source_image_sub_rectangle, depth, 0, unpack_image_height,
                  format, type,
                  premultiply_alpha ? kAlphaDoPremultiply : kAlphaDoNothing,
                  data.data(), flip_y))
    return false;

   return true;
 }
