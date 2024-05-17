size_t GLES2Util::GetGLTypeSizeForTextures(uint32_t type) {
  return static_cast<size_t>(BytesPerElement(type));
}
