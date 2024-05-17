error::Error GLES2DecoderPassthroughImpl::DoUniform3iv(
    GLint location,
    GLsizei count,
    const volatile GLint* v) {
  api()->glUniform3ivFn(location, count, const_cast<const GLint*>(v));
  return error::kNoError;
}
