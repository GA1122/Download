size_t GLES2Util::GetGroupSizeForBufferType(uint32_t count, uint32_t type) {
  size_t type_size = GetGLTypeSizeForBuffers(type);
  if (type == GL_INT_2_10_10_10_REV || type == GL_UNSIGNED_INT_2_10_10_10_REV) {
    DCHECK_EQ(4u, count);
    return type_size;
  }
  return type_size * count;
}
