void WebGLRenderingContextBase::copyTexImage2D(GLenum target,
                                               GLint level,
                                               GLenum internalformat,
                                               GLint x,
                                               GLint y,
                                               GLsizei width,
                                               GLsizei height,
                                               GLint border) {
  if (isContextLost())
    return;
  if (!ValidateTexture2DBinding("copyTexImage2D", target))
    return;
  if (!ValidateCopyTexFormat("copyTexImage2D", internalformat))
    return;
  if (!ValidateSettableTexFormat("copyTexImage2D", internalformat))
    return;
  WebGLFramebuffer* read_framebuffer_binding = nullptr;
  if (!ValidateReadBufferAndGetInfo("copyTexImage2D", read_framebuffer_binding))
    return;
  ClearIfComposited();
  ScopedDrawingBufferBinder binder(GetDrawingBuffer(),
                                   read_framebuffer_binding);
  ContextGL()->CopyTexImage2D(target, level, internalformat, x, y, width,
                              height, border);
}
