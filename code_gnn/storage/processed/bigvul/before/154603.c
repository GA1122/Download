error::Error GLES2DecoderPassthroughImpl::DoCopyTexSubImage2D(GLenum target,
                                                              GLint level,
                                                              GLint xoffset,
                                                              GLint yoffset,
                                                              GLint x,
                                                              GLint y,
                                                              GLsizei width,
                                                              GLsizei height) {
  api()->glCopyTexSubImage2DFn(target, level, xoffset, yoffset, x, y, width,
                               height);

  ExitCommandProcessingEarly();

  return error::kNoError;
}
