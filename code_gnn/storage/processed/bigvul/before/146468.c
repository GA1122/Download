void WebGLRenderingContextBase::copyTexSubImage2D(GLenum target,
                                                  GLint level,
                                                  GLint xoffset,
                                                  GLint yoffset,
                                                  GLint x,
                                                  GLint y,
                                                  GLsizei width,
                                                  GLsizei height) {
  if (isContextLost())
    return;
  if (!ValidateTexture2DBinding("copyTexSubImage2D", target))
    return;
  WebGLFramebuffer* read_framebuffer_binding = nullptr;
  if (!ValidateReadBufferAndGetInfo("copyTexSubImage2D",
                                    read_framebuffer_binding))
    return;
  ClearIfComposited();
  ScopedDrawingBufferBinder binder(GetDrawingBuffer(),
                                   read_framebuffer_binding);
  ContextGL()->CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width,
                                 height);
}
