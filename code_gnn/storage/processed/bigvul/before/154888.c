error::Error GLES2DecoderPassthroughImpl::DoWindowRectanglesEXT(
    GLenum mode,
    GLsizei n,
    const volatile GLint* box) {
  std::vector<GLint> box_copy(box, box + (n * 4));
  api()->glWindowRectanglesEXTFn(mode, n, box_copy.data());
  return error::kNoError;
}
