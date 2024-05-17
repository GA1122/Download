void WebGLRenderingContextBase::TexImageViaGPU(
    TexImageFunctionID function_id,
    WebGLTexture* texture,
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLint zoffset,
    AcceleratedStaticBitmapImage* source_image,
    WebGLRenderingContextBase* source_canvas_webgl_context,
    const IntRect& source_sub_rectangle,
    bool premultiply_alpha,
    bool flip_y) {
  bool have_source_image = source_image;
  bool have_source_canvas_webgl_context = source_canvas_webgl_context;
  DCHECK(have_source_image ^ have_source_canvas_webgl_context);

  int width = source_sub_rectangle.Width();
  int height = source_sub_rectangle.Height();

  ScopedTexture2DRestorer restorer(this);

  GLuint target_texture = texture->Object();
  bool possible_direct_copy = false;
  if (function_id == kTexImage2D || function_id == kTexSubImage2D) {
    possible_direct_copy = Extensions3DUtil::CanUseCopyTextureCHROMIUM(target);
  }

  GLint copy_x_offset = xoffset;
  GLint copy_y_offset = yoffset;
  GLenum copy_target = target;

  if (!possible_direct_copy) {
    ContextGL()->GenTextures(1, &target_texture);
    ContextGL()->BindTexture(GL_TEXTURE_2D, target_texture);
    ContextGL()->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                               GL_NEAREST);
    ContextGL()->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                               GL_NEAREST);
    ContextGL()->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                               GL_CLAMP_TO_EDGE);
    ContextGL()->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                               GL_CLAMP_TO_EDGE);
    ContextGL()->TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                            GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    copy_x_offset = 0;
    copy_y_offset = 0;
    copy_target = GL_TEXTURE_2D;
  }

  {
    ScopedUnpackParametersResetRestore temporaryResetUnpack(this);
    if (source_image) {
      source_image->CopyToTexture(
          ContextGL(), target, target_texture, level, premultiply_alpha, flip_y,
          IntPoint(xoffset, yoffset), source_sub_rectangle);
    } else {
      WebGLRenderingContextBase* gl = source_canvas_webgl_context;
      if (gl->is_origin_top_left_ && !canvas()->LowLatencyEnabled())
        flip_y = !flip_y;
      ScopedTexture2DRestorer inner_restorer(gl);
      if (!gl->GetDrawingBuffer()->CopyToPlatformTexture(
              ContextGL(), target, target_texture, level,
              unpack_premultiply_alpha_, !flip_y, IntPoint(xoffset, yoffset),
              source_sub_rectangle, kBackBuffer)) {
        NOTREACHED();
      }
    }
  }

  if (!possible_direct_copy) {
    GLuint tmp_fbo;
    ContextGL()->GenFramebuffers(1, &tmp_fbo);
    ContextGL()->BindFramebuffer(GL_FRAMEBUFFER, tmp_fbo);
    ContextGL()->FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                      GL_TEXTURE_2D, target_texture, 0);
    ContextGL()->BindTexture(texture->GetTarget(), texture->Object());
    if (function_id == kTexImage2D) {
      ContextGL()->CopyTexSubImage2D(target, level, 0, 0, 0, 0, width, height);
    } else if (function_id == kTexSubImage2D) {
      ContextGL()->CopyTexSubImage2D(target, level, xoffset, yoffset, 0, 0,
                                     width, height);
    } else if (function_id == kTexSubImage3D) {
      ContextGL()->CopyTexSubImage3D(target, level, xoffset, yoffset, zoffset,
                                     0, 0, width, height);
    }
    ContextGL()->FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                      GL_TEXTURE_2D, 0, 0);
    RestoreCurrentFramebuffer();
    ContextGL()->DeleteFramebuffers(1, &tmp_fbo);
    ContextGL()->DeleteTextures(1, &target_texture);
  }
}
