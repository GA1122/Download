error::Error GLES2DecoderPassthroughImpl::DoClearBufferuiv(
    GLenum buffer,
    GLint drawbuffers,
    const volatile GLuint* value) {
  api()->glClearBufferuivFn(buffer, drawbuffers,
                            const_cast<const GLuint*>(value));
  return error::kNoError;
}
