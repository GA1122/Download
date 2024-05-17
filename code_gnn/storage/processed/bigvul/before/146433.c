bool WebGLRenderingContextBase::ValidateUniformMatrixParameters(
    const char* function_name,
    const WebGLUniformLocation* location,
    GLboolean transpose,
    DOMFloat32Array* v,
    GLsizei required_min_size,
    GLuint src_offset,
    GLuint src_length) {
  if (!v) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name, "no array");
    return false;
  }
  return ValidateUniformMatrixParameters(
      function_name, location, transpose, v->DataMaybeShared(), v->length(),
      required_min_size, src_offset, src_length);
}
