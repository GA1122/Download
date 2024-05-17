error::Error GLES2DecoderPassthroughImpl::DoCopyTexSubImage3D(GLenum target,
                                                              GLint level,
                                                              GLint xoffset,
                                                              GLint yoffset,
                                                              GLint zoffset,
                                                              GLint x,
                                                              GLint y,
                                                              GLsizei width,
                                                              GLsizei height) {
  api()->glCopyTexSubImage3DFn(target, level, xoffset, yoffset, zoffset, x, y,
                               width, height);

  ExitCommandProcessingEarly();

  return error::kNoError;
}
