void WebGL2RenderingContextBase::readPixels(GLint x,
                                            GLint y,
                                            GLsizei width,
                                            GLsizei height,
                                            GLenum format,
                                            GLenum type,
                                            long long offset) {
  if (isContextLost())
    return;

  DCHECK(canvas()->OriginClean());

  if (!ValidateValueFitNonNegInt32("readPixels", "offset", offset))
    return;

  WebGLBuffer* buffer = bound_pixel_pack_buffer_.Get();
  if (!buffer) {
    SynthesizeGLError(GL_INVALID_OPERATION, "readPixels",
                      "no PIXEL_PACK buffer bound");
    return;
  }

  const char* reason = "framebuffer incomplete";
  WebGLFramebuffer* framebuffer = GetReadFramebufferBinding();
  if (framebuffer && framebuffer->CheckDepthStencilStatus(&reason) !=
                         GL_FRAMEBUFFER_COMPLETE) {
    SynthesizeGLError(GL_INVALID_FRAMEBUFFER_OPERATION, "readPixels", reason);
    return;
  }

  long long size = buffer->GetSize() - offset;
  if (!ValidateReadPixelsFuncParameters(width, height, format, type, nullptr,
                                        size))
    return;

  ClearIfComposited();

  {
    ScopedDrawingBufferBinder binder(GetDrawingBuffer(), framebuffer);
    ContextGL()->ReadPixels(x, y, width, height, format, type,
                            reinterpret_cast<void*>(offset));
  }
}
