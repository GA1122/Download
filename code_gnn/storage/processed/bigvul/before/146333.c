GLint WebGLRenderingContextBase::MaxColorAttachments() {
  if (isContextLost() ||
      !(ExtensionEnabled(kWebGLDrawBuffersName) || IsWebGL2OrHigher()))
    return 0;
  if (!max_color_attachments_)
    ContextGL()->GetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT,
                             &max_color_attachments_);
  return max_color_attachments_;
}
