bool ModifyAttachmentForEmulatedFramebuffer(GLenum* attachment) {
  switch (*attachment) {
    case GL_BACK:
      *attachment = GL_COLOR_ATTACHMENT0;
      return true;

    case GL_DEPTH:
      *attachment = GL_DEPTH_ATTACHMENT;
      return true;

    case GL_STENCIL:
      *attachment = GL_STENCIL_ATTACHMENT;
      return true;

    default:
      return false;
  }
}
