std::unique_ptr<uint8_t[]> DecompressTextureData(
    const ContextState& state,
    const CompressedFormatInfo& info,
    uint32_t width,
    uint32_t height,
    uint32_t depth,
    GLsizei image_size,
    const void* data) {
  auto* api = state.api();
  uint32_t output_pixel_size = GLES2Util::ComputeImageGroupSize(
      info.decompressed_format, info.decompressed_type);
  std::unique_ptr<uint8_t[]> decompressed_data(
      new uint8_t[output_pixel_size * width * height]);

  const void* input_data = data;
  if (state.bound_pixel_unpack_buffer) {
    input_data = api->glMapBufferRangeFn(GL_PIXEL_UNPACK_BUFFER,
                                         reinterpret_cast<GLintptr>(data),
                                         image_size, GL_MAP_READ_BIT);
    if (input_data == nullptr) {
      LOG(ERROR) << "Failed to map pixel unpack buffer.";
      return nullptr;
    }
  }

  DCHECK_NE(input_data, nullptr);
  info.decompression_function(
      width, height, depth, static_cast<const uint8_t*>(input_data),
      GetCompressedFormatRowPitch(info, width),
      GetCompressedFormatDepthPitch(info, width, height),
      decompressed_data.get(), output_pixel_size * width,
      output_pixel_size * width * height);

  if (state.bound_pixel_unpack_buffer) {
    if (api->glUnmapBufferFn(GL_PIXEL_UNPACK_BUFFER) != GL_TRUE) {
      LOG(ERROR) << "glUnmapBuffer unexpectedly returned GL_FALSE";
      return nullptr;
    }
  }

  return decompressed_data;
}
