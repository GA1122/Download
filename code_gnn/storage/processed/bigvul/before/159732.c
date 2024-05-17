void WebGLRenderingContextBase::TexImageByGPU(
    TexImageFunctionID function_id,
    WebGLTexture* texture,
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLint zoffset,
    CanvasImageSource* image,
    const IntRect& source_sub_rectangle) {
  DCHECK(image->IsCanvasElement() || image->IsImageBitmap());
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
    if (image->IsCanvasElement()) {
      TexImageCanvasByGPU(function_id, static_cast<HTMLCanvasElement*>(image),
                          copy_target, target_texture, copy_x_offset,
                          copy_y_offset, source_sub_rectangle);
    } else {
      TexImageBitmapByGPU(static_cast<ImageBitmap*>(image), copy_target,
                          target_texture, copy_x_offset, copy_y_offset,
                          source_sub_rectangle);
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
