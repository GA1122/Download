bool GLES2Implementation::ValidateSize(const char* func, GLsizeiptr size) {
  if (size < 0) {
    SetGLError(GL_INVALID_VALUE, func, "size < 0");
    return false;
  }
  if (!base::IsValueInRangeForNumericType<int32_t>(size)) {
    SetGLError(GL_INVALID_OPERATION, func, "size more than 32-bit");
    return false;
  }
  return true;
}
