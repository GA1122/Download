bool GLES2DecoderImpl::GetNumValuesReturnedForGLGet(
    GLenum pname, GLsizei* num_values) {
  *num_values = 0;
  if (state_.GetStateAsGLint(pname, nullptr, num_values)) {
    return true;
  }
  return GetHelper(pname, nullptr, num_values);
}
