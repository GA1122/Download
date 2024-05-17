void WebGL2RenderingContextBase::blitFramebuffer(GLint src_x0,
                                                 GLint src_y0,
                                                 GLint src_x1,
                                                 GLint src_y1,
                                                 GLint dst_x0,
                                                 GLint dst_y0,
                                                 GLint dst_x1,
                                                 GLint dst_y1,
                                                 GLbitfield mask,
                                                 GLenum filter) {
  if (isContextLost())
    return;

  DrawingBuffer::ScopedRGBEmulationForBlitFramebuffer emulation(
      GetDrawingBuffer());
  ContextGL()->BlitFramebufferCHROMIUM(src_x0, src_y0, src_x1, src_y1, dst_x0,
                                       dst_y0, dst_x1, dst_y1, mask, filter);
}
