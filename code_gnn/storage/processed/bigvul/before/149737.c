void FeatureInfo::InitializeFloatAndHalfFloatFeatures(
    const gfx::ExtensionSet& extensions) {
  bool enable_texture_float = false;
  bool enable_texture_float_linear = false;
  bool enable_texture_half_float = false;
  bool enable_texture_half_float_linear = false;
  bool enable_ext_color_buffer_float = false;
  bool enable_ext_color_buffer_half_float = false;

  bool may_enable_chromium_color_buffer_float = false;

  bool enable_es3 = IsWebGL2OrES3OrHigherContext();

  if (gfx::HasExtension(extensions, "GL_EXT_color_buffer_float"))
    enable_ext_color_buffer_float = true;
  if (gfx::HasExtension(extensions, "GL_EXT_color_buffer_half_float")) {
    enable_ext_color_buffer_half_float = true;
  }

  if (gfx::HasExtension(extensions, "GL_ARB_texture_float") ||
      gl_version_info_->is_desktop_core_profile) {
    enable_texture_float = true;
    enable_texture_float_linear = true;
    enable_texture_half_float = true;
    enable_texture_half_float_linear = true;
    may_enable_chromium_color_buffer_float = true;
  } else {
    if (gfx::HasExtension(extensions, "GL_OES_texture_float")) {
      enable_texture_float = true;
      if (enable_ext_color_buffer_float) {
        may_enable_chromium_color_buffer_float = true;
      }
    }

    if (gfx::HasExtension(extensions, "GL_OES_texture_float_linear")) {
      enable_texture_float_linear = true;
    }

    if (gfx::HasExtension(extensions, "GL_OES_texture_half_float")) {
      enable_texture_half_float = true;
    }

    if (gfx::HasExtension(extensions, "GL_OES_texture_half_float_linear")) {
      enable_texture_half_float_linear = true;
    }
  }

  if (enable_texture_float) {
    validators_.pixel_type.AddValue(GL_FLOAT);
    validators_.read_pixel_type.AddValue(GL_FLOAT);
    AddExtensionString("GL_OES_texture_float");
  }

  if (enable_texture_float_linear) {
    oes_texture_float_linear_available_ = true;
    if (!disallowed_features_.oes_texture_float_linear)
      EnableOESTextureFloatLinear();
  }

  if (enable_texture_half_float) {
    validators_.pixel_type.AddValue(GL_HALF_FLOAT_OES);
    validators_.read_pixel_type.AddValue(GL_HALF_FLOAT_OES);
    AddExtensionString("GL_OES_texture_half_float");
  }

  if (enable_texture_half_float_linear) {
    oes_texture_half_float_linear_available_ = true;
    if (!disallowed_features_.oes_texture_half_float_linear)
      EnableOESTextureHalfFloatLinear();
  }

  bool had_native_chromium_color_buffer_float_ext = false;
  if (gfx::HasExtension(extensions, "GL_CHROMIUM_color_buffer_float_rgb")) {
    had_native_chromium_color_buffer_float_ext = true;
    feature_flags_.chromium_color_buffer_float_rgb = true;
    if (!disallowed_features_.chromium_color_buffer_float_rgb) {
      EnableCHROMIUMColorBufferFloatRGB();
    }
  }

  if (gfx::HasExtension(extensions, "GL_CHROMIUM_color_buffer_float_rgba")) {
    had_native_chromium_color_buffer_float_ext = true;
    feature_flags_.chromium_color_buffer_float_rgba = true;
    if (!disallowed_features_.chromium_color_buffer_float_rgba) {
      EnableCHROMIUMColorBufferFloatRGBA();
    }
  }

  if (!gl_version_info_->is_es || gl_version_info_->IsAtLeastGLES(3, 2) ||
      gfx::HasExtension(extensions, "GL_EXT_float_blend")) {
    if (!disallowed_features_.ext_float_blend) {
      EnableEXTFloatBlend();
    }
  }

  if (may_enable_chromium_color_buffer_float &&
      !had_native_chromium_color_buffer_float_ext) {
    static_assert(GL_RGBA32F_ARB == GL_RGBA32F &&
                      GL_RGBA32F_EXT == GL_RGBA32F &&
                      GL_RGB32F_ARB == GL_RGB32F && GL_RGB32F_EXT == GL_RGB32F,
                  "sized float internal format variations must match");
    GLint fb_binding = 0;
    GLint tex_binding = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb_binding);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex_binding);

    GLuint tex_id = 0;
    GLuint fb_id = 0;
    GLsizei width = 16;

    glGenTextures(1, &tex_id);
    glGenFramebuffersEXT(1, &fb_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, width, 0, GL_RGBA,
                 GL_FLOAT, nullptr);
    glBindFramebufferEXT(GL_FRAMEBUFFER, fb_id);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_TEXTURE_2D, tex_id, 0);
    GLenum status_rgba = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, width, 0, GL_RGB, GL_FLOAT,
                 nullptr);
    GLenum status_rgb = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);

    if (status_rgba == GL_FRAMEBUFFER_COMPLETE && enable_es3) {
      bool full_float_support = true;
      GLenum internal_formats[] = {
          GL_R16F, GL_RG16F, GL_RGBA16F, GL_R32F, GL_RG32F, GL_R11F_G11F_B10F,
      };
      GLenum formats[] = {
          GL_RED, GL_RG, GL_RGBA, GL_RED, GL_RG, GL_RGB,
      };
      DCHECK_EQ(base::size(internal_formats), base::size(formats));
      for (size_t i = 0; i < base::size(formats); ++i) {
        glTexImage2D(GL_TEXTURE_2D, 0, internal_formats[i], width, width, 0,
                     formats[i], GL_FLOAT, nullptr);
        full_float_support &= glCheckFramebufferStatusEXT(GL_FRAMEBUFFER) ==
                              GL_FRAMEBUFFER_COMPLETE;
      }
      enable_ext_color_buffer_float = full_float_support;
    }
    if (IsWebGL1OrES2Context() && !enable_ext_color_buffer_half_float &&
        (gl_version_info_->IsAtLeastGLES(3, 0) ||
         gl_version_info_->IsAtLeastGL(3, 0))) {
      GLenum internal_format = GL_RGBA16F;
      GLenum format = GL_RGBA;
      GLenum data_type = GL_HALF_FLOAT;
      glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, width, 0, format,
                   data_type, nullptr);
      enable_ext_color_buffer_half_float =
          (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER) ==
           GL_FRAMEBUFFER_COMPLETE);
    }

    glDeleteFramebuffersEXT(1, &fb_id);
    glDeleteTextures(1, &tex_id);

    glBindFramebufferEXT(GL_FRAMEBUFFER, static_cast<GLuint>(fb_binding));
    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(tex_binding));

    DCHECK_EQ(glGetError(), static_cast<GLuint>(GL_NO_ERROR));

    if (status_rgba == GL_FRAMEBUFFER_COMPLETE) {
      feature_flags_.chromium_color_buffer_float_rgba = true;
      if (!disallowed_features_.chromium_color_buffer_float_rgba)
        EnableCHROMIUMColorBufferFloatRGBA();
    }
    if (status_rgb == GL_FRAMEBUFFER_COMPLETE) {
      feature_flags_.chromium_color_buffer_float_rgb = true;
      if (!disallowed_features_.chromium_color_buffer_float_rgb)
        EnableCHROMIUMColorBufferFloatRGB();
    }
  }

  if (enable_ext_color_buffer_float && enable_es3) {
    ext_color_buffer_float_available_ = true;
    if (!disallowed_features_.ext_color_buffer_float)
      EnableEXTColorBufferFloat();
  }

  if (enable_ext_color_buffer_half_float) {
    ext_color_buffer_half_float_available_ = true;
    if (!disallowed_features_.ext_color_buffer_half_float)
      EnableEXTColorBufferHalfFloat();
  }

  if (enable_texture_float) {
    validators_.texture_internal_format_storage.AddValue(GL_RGBA32F_EXT);
    validators_.texture_internal_format_storage.AddValue(GL_RGB32F_EXT);
    validators_.texture_internal_format_storage.AddValue(GL_ALPHA32F_EXT);
    validators_.texture_internal_format_storage.AddValue(GL_LUMINANCE32F_EXT);
    validators_.texture_internal_format_storage.AddValue(
        GL_LUMINANCE_ALPHA32F_EXT);
  }

  if (enable_texture_half_float) {
    validators_.texture_internal_format_storage.AddValue(GL_RGBA16F_EXT);
    validators_.texture_internal_format_storage.AddValue(GL_RGB16F_EXT);
    validators_.texture_internal_format_storage.AddValue(GL_ALPHA16F_EXT);
    validators_.texture_internal_format_storage.AddValue(GL_LUMINANCE16F_EXT);
    validators_.texture_internal_format_storage.AddValue(
        GL_LUMINANCE_ALPHA16F_EXT);
  }

  g_l16_is_present =
      enable_texture_half_float && feature_flags_.ext_texture_storage;
}
