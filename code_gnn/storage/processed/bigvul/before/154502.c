void ResizeRenderbuffer(gl::GLApi* api,
                        GLuint renderbuffer,
                        const gfx::Size& size,
                        GLsizei samples,
                        GLenum internal_format,
                        const FeatureInfo* feature_info) {
  ScopedRenderbufferBindingReset scoped_renderbuffer_reset(api);

  api->glBindRenderbufferEXTFn(GL_RENDERBUFFER, renderbuffer);
  if (samples > 0) {
    DCHECK(feature_info->feature_flags().chromium_framebuffer_multisample);
    api->glRenderbufferStorageMultisampleFn(
        GL_RENDERBUFFER, samples, internal_format, size.width(), size.height());
  } else {
    api->glRenderbufferStorageEXTFn(GL_RENDERBUFFER, internal_format,
                                    size.width(), size.height());
  }
}
