error::Error GLES2DecoderPassthroughImpl::DoTexParameteri(GLenum target,
                                                          GLenum pname,
                                                          GLint param) {
  api()->glTexParameteriFn(target, pname, param);
  return error::kNoError;
}
