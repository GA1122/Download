bool GLES2Implementation::ValidateOffset(const char* func, GLintptr offset) {
  if (offset < 0) {
    SetGLError(GL_INVALID_VALUE, func, "offset < 0");
    return false;
  }
  if (!base::IsValueInRangeForNumericType<int32_t>(offset)) {
    SetGLError(GL_INVALID_OPERATION, func, "offset more than 32-bit");
    return false;
  }
  return true;
}
