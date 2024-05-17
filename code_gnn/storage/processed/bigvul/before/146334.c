GLint WebGLRenderingContextBase::MaxDrawBuffers() {
  if (isContextLost() ||
      !(ExtensionEnabled(kWebGLDrawBuffersName) || IsWebGL2OrHigher()))
    return 0;
  if (!max_draw_buffers_)
    ContextGL()->GetIntegerv(GL_MAX_DRAW_BUFFERS_EXT, &max_draw_buffers_);
  if (!max_color_attachments_)
    ContextGL()->GetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT,
                             &max_color_attachments_);
  return std::min(max_draw_buffers_, max_color_attachments_);
}
