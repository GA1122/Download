error::Error GLES2DecoderPassthroughImpl::DoCopyTexImage2D(
    GLenum target,
    GLint level,
    GLenum internalformat,
    GLint x,
    GLint y,
    GLsizei width,
    GLsizei height,
    GLint border) {
  CheckErrorCallbackState();
  api()->glCopyTexImage2DFn(target, level, internalformat, x, y, width, height,
                            border);
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  UpdateTextureSizeFromTarget(target);

  ExitCommandProcessingEarly();

  return error::kNoError;
}
