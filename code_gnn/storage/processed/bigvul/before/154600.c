error::Error GLES2DecoderPassthroughImpl::DoCopyBufferSubData(
    GLenum readtarget,
    GLenum writetarget,
    GLintptr readoffset,
    GLintptr writeoffset,
    GLsizeiptr size) {
  api()->glCopyBufferSubDataFn(readtarget, writetarget, readoffset, writeoffset,
                               size);
  return error::kNoError;
}
