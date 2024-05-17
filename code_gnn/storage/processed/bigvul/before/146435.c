bool WebGLRenderingContextBase::ValidateUniformParameters(
    const char* function_name,
    const WebGLUniformLocation* location,
    void* v,
    GLsizei size,
    GLsizei required_min_size,
    GLuint src_offset,
    GLuint src_length) {
  return ValidateUniformMatrixParameters(function_name, location, false, v,
                                         size, required_min_size, src_offset,
                                         src_length);
}
