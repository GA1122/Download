error::Error GLES2DecoderPassthroughImpl::DoDepthRangef(GLclampf zNear,
                                                        GLclampf zFar) {
  api()->glDepthRangefFn(zNear, zFar);
  return error::kNoError;
}
