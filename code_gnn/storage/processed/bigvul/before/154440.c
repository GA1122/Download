GLES2DecoderPassthroughImpl::EmulatedColorBuffer::EmulatedColorBuffer(
    gl::GLApi* api,
    const EmulatedDefaultFramebufferFormat& format_in)
    : api(api), format(format_in) {
  ScopedTexture2DBindingReset scoped_texture_reset(api);

  GLuint color_buffer_texture = 0;
  api->glGenTexturesFn(1, &color_buffer_texture);
  api->glBindTextureFn(GL_TEXTURE_2D, color_buffer_texture);
  api->glTexParameteriFn(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  api->glTexParameteriFn(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  api->glTexParameteriFn(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  api->glTexParameteriFn(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  texture = new TexturePassthrough(color_buffer_texture, GL_TEXTURE_2D);
}
