error::Error GLES2DecoderPassthroughImpl::DoColorMask(GLboolean red,
                                                      GLboolean green,
                                                      GLboolean blue,
                                                      GLboolean alpha) {
  api()->glColorMaskFn(red, green, blue, alpha);
  return error::kNoError;
}
