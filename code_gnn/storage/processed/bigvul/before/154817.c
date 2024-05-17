error::Error GLES2DecoderPassthroughImpl::DoTexParameterf(GLenum target,
                                                          GLenum pname,
                                                          GLfloat param) {
  api()->glTexParameterfFn(target, pname, param);
  return error::kNoError;
}
