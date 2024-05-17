bool WebGL2RenderingContextBase::CheckAndTranslateAttachments(
    const char* function_name,
    GLenum target,
    Vector<GLenum>& attachments) {
  if (!ValidateFramebufferTarget(target)) {
    SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid target");
    return false;
  }

  WebGLFramebuffer* framebuffer_binding = GetFramebufferBinding(target);
  DCHECK(framebuffer_binding || GetDrawingBuffer());
  if (!framebuffer_binding) {
    for (size_t i = 0; i < attachments.size(); ++i) {
      switch (attachments[i]) {
        case GL_COLOR:
          attachments[i] = GL_COLOR_ATTACHMENT0;
          break;
        case GL_DEPTH:
          attachments[i] = GL_DEPTH_ATTACHMENT;
          break;
        case GL_STENCIL:
          attachments[i] = GL_STENCIL_ATTACHMENT;
          break;
        default:
          SynthesizeGLError(GL_INVALID_ENUM, function_name,
                            "invalid attachment");
          return false;
      }
    }
  }
  return true;
}
