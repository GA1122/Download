void GLES2DecoderImpl::DoGetFramebufferAttachmentParameteriv(
    GLenum target,
    GLenum attachment,
    GLenum pname,
    GLint* params,
    GLsizei params_size) {
  const char kFunctionName[] = "glGetFramebufferAttachmentParameteriv";
  Framebuffer* framebuffer = GetFramebufferInfoForTarget(target);
  if (!framebuffer) {
    if (!feature_info_->IsWebGL2OrES3Context()) {
      LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName,
          "no framebuffer bound");
      return;
    }
    if (!validators_->backbuffer_attachment.IsValid(attachment)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName,
          "invalid attachment for backbuffer");
      return;
    }
    switch (pname) {
      case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
        *params = static_cast<GLint>(GL_FRAMEBUFFER_DEFAULT);
        return;
      case GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE:
      case GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE:
      case GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE:
      case GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE:
      case GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE:
      case GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE:
      case GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE:
      case GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING:
        break;
      default:
        LOCAL_SET_GL_ERROR(GL_INVALID_ENUM, kFunctionName,
            "invalid pname for backbuffer");
        return;
    }
    if (GetBackbufferServiceId() != 0) {   
      switch (attachment) {
        case GL_BACK:
          attachment = GL_COLOR_ATTACHMENT0;
          break;
        case GL_DEPTH:
          attachment = GL_DEPTH_ATTACHMENT;
          break;
        case GL_STENCIL:
          attachment = GL_STENCIL_ATTACHMENT;
          break;
        default:
          NOTREACHED();
          break;
      }
    }
  } else {
    if (attachment == GL_DEPTH_STENCIL_ATTACHMENT) {
      const Framebuffer::Attachment* depth =
          framebuffer->GetAttachment(GL_DEPTH_ATTACHMENT);
      const Framebuffer::Attachment* stencil =
          framebuffer->GetAttachment(GL_STENCIL_ATTACHMENT);
      if ((!depth && !stencil) ||
          (depth && stencil && depth->IsSameAttachment(stencil))) {
        attachment = GL_DEPTH_ATTACHMENT;
      } else {
        LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName,
                           "depth and stencil attachment mismatch");
        return;
      }
    }
  }
  if (pname == GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_SAMPLES_EXT &&
      features().use_img_for_multisampled_render_to_texture) {
    pname = GL_TEXTURE_SAMPLES_IMG;
  }
  if (pname == GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME) {
    DCHECK(framebuffer);
    const Framebuffer::Attachment* attachment_object =
        framebuffer->GetAttachment(attachment);
    *params = attachment_object ? attachment_object->object_name() : 0;
    return;
  }

  api()->glGetFramebufferAttachmentParameterivEXTFn(target, attachment, pname,
                                                    params);
  LOCAL_PEEK_GL_ERROR(kFunctionName);
}
