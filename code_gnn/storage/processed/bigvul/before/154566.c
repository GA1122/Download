error::Error GLES2DecoderPassthroughImpl::DoBindImageTexture(GLuint unit,
                                                             GLuint texture,
                                                             GLint level,
                                                             GLboolean layered,
                                                             GLint layer,
                                                             GLenum access,
                                                             GLenum format) {
  api()->glBindImageTextureEXTFn(
      unit,
      GetTextureServiceID(api(), texture, resources_, bind_generates_resource_),
      level, layered, layer, access, format);
  return error::kNoError;
}
