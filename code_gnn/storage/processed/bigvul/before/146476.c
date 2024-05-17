void WebGLRenderingContextBase::deleteBuffer(WebGLBuffer* buffer) {
  if (!DeleteObject(buffer))
    return;
  RemoveBoundBuffer(buffer);
}
