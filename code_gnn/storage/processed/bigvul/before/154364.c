bool GLES2DecoderImpl::SetVertexAttribValue(
    const char* function_name, GLuint index, const T* value) {
  if (index >= state_.attrib_values.size()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "index out of range");
    return false;
  }
  state_.attrib_values[index].SetValues(value);
  return true;
}
