error::Error GLES2DecoderPassthroughImpl::DoBindRenderbuffer(
    GLenum target,
    GLuint renderbuffer) {
  api()->glBindRenderbufferEXTFn(
      target, GetRenderbufferServiceID(api(), renderbuffer, resources_,
                                       bind_generates_resource_));
  return error::kNoError;
}
