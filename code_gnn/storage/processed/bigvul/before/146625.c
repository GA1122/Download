GLenum DrawingBuffer::GetMultisampledRenderbufferFormat() {
  DCHECK(WantExplicitResolve());
  if (want_alpha_channel_)
    return GL_RGBA8_OES;
  if (ShouldUseChromiumImage() &&
      ContextProvider()->GetCapabilities().chromium_image_rgb_emulation)
    return GL_RGBA8_OES;
  if (ContextProvider()
          ->GetCapabilities()
          .disable_webgl_rgb_multisampling_usage)
    return GL_RGBA8_OES;
  return GL_RGB8_OES;
}
