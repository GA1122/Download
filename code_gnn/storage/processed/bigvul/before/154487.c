GLES2DecoderPassthroughImpl::PatchGetFramebufferAttachmentParameter(
    GLenum target,
    GLenum attachment,
    GLenum pname,
    GLsizei length,
    GLint* params) {
  if (length < 1) {
    return error::kNoError;
  }

  switch (pname) {
    case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME: {
      GLint object_type = GL_NONE;
      api()->glGetFramebufferAttachmentParameterivEXTFn(
          target, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
          &object_type);

      switch (object_type) {
        case GL_TEXTURE:
          if (!GetClientID(&resources_->texture_id_map, *params, params)) {
            return error::kInvalidArguments;
          }
          break;

        case GL_RENDERBUFFER:
          if (!GetClientID(&resources_->renderbuffer_id_map, *params, params)) {
            return error::kInvalidArguments;
          }
          break;

        case GL_NONE:
          break;

        default:
          NOTREACHED();
          break;
      }
    } break;

    case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
      if (IsEmulatedFramebufferBound(target)) {
        *params = GL_FRAMEBUFFER_DEFAULT;
      }
      break;

    default:
      break;
  }

  return error::kNoError;
}
