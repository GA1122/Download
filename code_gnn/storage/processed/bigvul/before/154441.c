    EmulatedDefaultFramebuffer(
        gl::GLApi* api,
        const EmulatedDefaultFramebufferFormat& format_in,
        const FeatureInfo* feature_info,
        bool supports_separate_fbo_bindings_in)
    : api(api),
      supports_separate_fbo_bindings(supports_separate_fbo_bindings_in),
      format(format_in) {
  ScopedFramebufferBindingReset scoped_fbo_reset(
      api, supports_separate_fbo_bindings);
  ScopedRenderbufferBindingReset scoped_renderbuffer_reset(api);

  api->glGenFramebuffersEXTFn(1, &framebuffer_service_id);
  api->glBindFramebufferEXTFn(GL_FRAMEBUFFER, framebuffer_service_id);

  if (format.samples > 0) {
    api->glGenRenderbuffersEXTFn(1, &color_buffer_service_id);
    api->glBindRenderbufferEXTFn(GL_RENDERBUFFER, color_buffer_service_id);
    api->glFramebufferRenderbufferEXTFn(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                        GL_RENDERBUFFER,
                                        color_buffer_service_id);
  } else {
    color_texture.reset(new EmulatedColorBuffer(api, format));
    api->glFramebufferTexture2DEXTFn(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                     GL_TEXTURE_2D,
                                     color_texture->texture->service_id(), 0);
  }

  if (format.depth_stencil_internal_format != GL_NONE) {
    DCHECK(format.depth_internal_format == GL_NONE &&
           format.stencil_internal_format == GL_NONE);
    api->glGenRenderbuffersEXTFn(1, &depth_stencil_buffer_service_id);
    api->glBindRenderbufferEXTFn(GL_RENDERBUFFER,
                                 depth_stencil_buffer_service_id);
    if (feature_info->gl_version_info().IsAtLeastGLES(3, 0) ||
        feature_info->feature_flags().angle_webgl_compatibility) {
      api->glFramebufferRenderbufferEXTFn(
          GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
          depth_stencil_buffer_service_id);
    } else {
      api->glFramebufferRenderbufferEXTFn(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                          GL_RENDERBUFFER,
                                          depth_stencil_buffer_service_id);
      api->glFramebufferRenderbufferEXTFn(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
                                          GL_RENDERBUFFER,
                                          depth_stencil_buffer_service_id);
    }
  } else {
    if (format.depth_internal_format != GL_NONE) {
      api->glGenRenderbuffersEXTFn(1, &depth_buffer_service_id);
      api->glBindRenderbufferEXTFn(GL_RENDERBUFFER, depth_buffer_service_id);
      api->glFramebufferRenderbufferEXTFn(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                          GL_RENDERBUFFER,
                                          depth_buffer_service_id);
    }

    if (format.stencil_internal_format != GL_NONE) {
      api->glGenRenderbuffersEXTFn(1, &stencil_buffer_service_id);
      api->glBindRenderbufferEXTFn(GL_RENDERBUFFER, stencil_buffer_service_id);
      api->glFramebufferRenderbufferEXTFn(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
                                          GL_RENDERBUFFER,
                                          stencil_buffer_service_id);
    }
  }
}
