void WebGLRenderingContextBase::ReadPixelsHelper(GLint x,
                                                 GLint y,
                                                 GLsizei width,
                                                 GLsizei height,
                                                 GLenum format,
                                                 GLenum type,
                                                 DOMArrayBufferView* pixels,
                                                 GLuint offset) {
  if (isContextLost())
    return;
  DCHECK(Host()->OriginClean());

  if (!pixels) {
    SynthesizeGLError(GL_INVALID_VALUE, "readPixels",
                      "no destination ArrayBufferView");
    return;
  }
  CheckedNumeric<GLuint> offset_in_bytes = offset;
  offset_in_bytes *= pixels->TypeSize();
  if (!offset_in_bytes.IsValid() ||
      offset_in_bytes.ValueOrDie() > pixels->byteLength()) {
    SynthesizeGLError(GL_INVALID_VALUE, "readPixels",
                      "destination offset out of range");
    return;
  }
  const char* reason = "framebuffer incomplete";
  WebGLFramebuffer* framebuffer = GetReadFramebufferBinding();
  if (framebuffer && framebuffer->CheckDepthStencilStatus(&reason) !=
                         GL_FRAMEBUFFER_COMPLETE) {
    SynthesizeGLError(GL_INVALID_FRAMEBUFFER_OPERATION, "readPixels", reason);
    return;
  }
  CheckedNumeric<GLuint> buffer_size = pixels->byteLength() - offset_in_bytes;
  if (!buffer_size.IsValid()) {
    SynthesizeGLError(GL_INVALID_VALUE, "readPixels",
                      "destination offset out of range");
    return;
  }
  if (!ValidateReadPixelsFuncParameters(width, height, format, type, pixels,
                                        buffer_size.ValueOrDie())) {
    return;
  }
  ClearIfComposited();
  uint8_t* data = static_cast<uint8_t*>(pixels->BaseAddressMaybeShared()) +
                  offset_in_bytes.ValueOrDie();
  {
    ScopedDrawingBufferBinder binder(GetDrawingBuffer(), framebuffer);
    ContextGL()->ReadPixels(x, y, width, height, format, type, data);
  }
}
