error::Error GLES2DecoderPassthroughImpl::DoPixelStorei(GLenum pname,
                                                        GLint param) {
  api()->glPixelStoreiFn(pname, param);
  return error::kNoError;
}
