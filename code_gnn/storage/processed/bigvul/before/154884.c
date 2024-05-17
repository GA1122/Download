error::Error GLES2DecoderPassthroughImpl::DoVertexAttribPointer(
    GLuint indx,
    GLint size,
    GLenum type,
    GLboolean normalized,
    GLsizei stride,
    const void* ptr) {
  api()->glVertexAttribPointerFn(indx, size, type, normalized, stride, ptr);
  return error::kNoError;
}
