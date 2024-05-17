bool IsWebGLDrawBuffersSupported(bool webglCompatibilityContext,
                                 GLenum depth_texture_internal_format,
                                 GLenum depth_stencil_texture_internal_format) {
  GLint max_draw_buffers = 0;
  GLint max_color_attachments = 0;
  glGetIntegerv(GL_MAX_DRAW_BUFFERS, &max_draw_buffers);
  glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max_color_attachments);
  if (max_draw_buffers < 4 || max_color_attachments < 4) {
    return false;
  }

  GLint fb_binding = 0;
  GLint tex_binding = 0;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb_binding);
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex_binding);

  GLuint fbo;
  glGenFramebuffersEXT(1, &fbo);
  glBindFramebufferEXT(GL_FRAMEBUFFER, fbo);

  GLuint depth_stencil_texture = 0;
  if (depth_stencil_texture_internal_format != GL_NONE) {
    glGenTextures(1, &depth_stencil_texture);
    glBindTexture(GL_TEXTURE_2D, depth_stencil_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, depth_stencil_texture_internal_format, 1, 1,
                 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
  }

  GLuint depth_texture = 0;
  if (depth_texture_internal_format != GL_NONE) {
    glGenTextures(1, &depth_texture);
    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, depth_texture_internal_format, 1, 1, 0,
                 GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);
  }

  GLint max_allowed_buffers = std::min(max_draw_buffers, max_color_attachments);
  std::vector<GLuint> colors(max_allowed_buffers, 0);
  glGenTextures(max_allowed_buffers, colors.data());

  bool result = true;
  for (GLint i = 0; i < max_allowed_buffers; ++i) {
    GLint color = colors[i];
    glBindTexture(GL_TEXTURE_2D, color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 nullptr);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                              GL_TEXTURE_2D, color, 0);
    if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER) !=
        GL_FRAMEBUFFER_COMPLETE) {
      result = false;
      break;
    }
    if (depth_texture != 0) {
      glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                GL_TEXTURE_2D, depth_texture, 0);
      if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER) !=
          GL_FRAMEBUFFER_COMPLETE) {
        result = false;
        break;
      }
      glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                GL_TEXTURE_2D, 0, 0);
    }
    if (depth_stencil_texture != 0) {
      if (webglCompatibilityContext) {
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                  GL_TEXTURE_2D, depth_stencil_texture, 0);
      } else {
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_TEXTURE_2D, depth_stencil_texture, 0);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
                                  GL_TEXTURE_2D, depth_stencil_texture, 0);
      }
      if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER) !=
          GL_FRAMEBUFFER_COMPLETE) {
        result = false;
        break;
      }
      if (webglCompatibilityContext) {
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                  GL_TEXTURE_2D, 0, 0);
      } else {
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_TEXTURE_2D, 0, 0);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
                                  GL_TEXTURE_2D, 0, 0);
      }
    }
  }

  glBindFramebufferEXT(GL_FRAMEBUFFER, static_cast<GLuint>(fb_binding));
  glDeleteFramebuffersEXT(1, &fbo);

  glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(tex_binding));
  glDeleteTextures(1, &depth_texture);
  glDeleteTextures(1, &depth_stencil_texture);
  glDeleteTextures(colors.size(), colors.data());

  DCHECK(glGetError() == GL_NO_ERROR);

  return result;
}
