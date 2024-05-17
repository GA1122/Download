error::Error GLES2DecoderPassthroughImpl::DoVertexAttribIPointer(
    GLuint indx,
    GLint size,
    GLenum type,
    GLsizei stride,
    const void* ptr) {
  api()->glVertexAttribIPointerFn(indx, size, type, stride, ptr);
  return error::kNoError;
}
