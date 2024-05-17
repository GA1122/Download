void WebGL2RenderingContextBase::clearBufferfi(GLenum buffer,
                                               GLint drawbuffer,
                                               GLfloat depth,
                                               GLint stencil) {
  if (isContextLost())
    return;

  ContextGL()->ClearBufferfi(buffer, drawbuffer, depth, stencil);
  MarkContextChanged(kCanvasChanged);
  UpdateBuffersToAutoClear(kClearBufferfi, buffer, drawbuffer);
}
