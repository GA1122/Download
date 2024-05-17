bool WebGLRenderingContextBase::ValidateReadPixelsFuncParameters(
    GLsizei width,
    GLsizei height,
    GLenum format,
    GLenum type,
    DOMArrayBufferView* buffer,
    long long buffer_size) {
  if (!ValidateReadPixelsFormatAndType(format, type, buffer))
    return false;

  unsigned total_bytes_required = 0, total_skip_bytes = 0;
  GLenum error = WebGLImageConversion::ComputeImageSizeInBytes(
      format, type, width, height, 1, GetPackPixelStoreParams(),
      &total_bytes_required, nullptr, &total_skip_bytes);
  if (error != GL_NO_ERROR) {
    SynthesizeGLError(error, "readPixels", "invalid dimensions");
    return false;
  }
  if (buffer_size <
      static_cast<long long>(total_bytes_required + total_skip_bytes)) {
    SynthesizeGLError(GL_INVALID_OPERATION, "readPixels",
                      "buffer is not large enough for dimensions");
    return false;
  }
  return true;
}
