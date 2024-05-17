bool WebGLRenderingContextBase::ValidateFramebufferFuncParameters(
    const char* function_name,
    GLenum target,
    GLenum attachment) {
  if (!ValidateFramebufferTarget(target)) {
    SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid target");
    return false;
  }
  switch (attachment) {
    case GL_COLOR_ATTACHMENT0:
    case GL_DEPTH_ATTACHMENT:
    case GL_STENCIL_ATTACHMENT:
    case GL_DEPTH_STENCIL_ATTACHMENT:
      break;
    default:
      if ((ExtensionEnabled(kWebGLDrawBuffersName) || IsWebGL2OrHigher()) &&
          attachment > GL_COLOR_ATTACHMENT0 &&
          attachment <
              static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + MaxColorAttachments()))
        break;
      SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid attachment");
      return false;
  }
  return true;
}
