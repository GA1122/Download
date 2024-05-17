void WebGL2RenderingContextBase::copyTexSubImage3D(GLenum target,
                                                   GLint level,
                                                   GLint xoffset,
                                                   GLint yoffset,
                                                   GLint zoffset,
                                                   GLint x,
                                                   GLint y,
                                                   GLsizei width,
                                                   GLsizei height) {
  if (isContextLost())
    return;
  if (!ValidateTexture3DBinding("copyTexSubImage3D", target))
    return;
  WebGLFramebuffer* read_framebuffer_binding = nullptr;
  if (!ValidateReadBufferAndGetInfo("copyTexSubImage3D",
                                    read_framebuffer_binding))
    return;
  ClearIfComposited();
  ScopedDrawingBufferBinder binder(GetDrawingBuffer(),
                                   read_framebuffer_binding);
  ContextGL()->CopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y,
                                 width, height);
}
