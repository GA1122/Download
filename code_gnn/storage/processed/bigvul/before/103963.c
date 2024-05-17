void FrameBuffer::Clear(GLbitfield buffers) {
  ScopedGLErrorSuppressor suppressor(decoder_);
  ScopedFrameBufferBinder binder(decoder_, id_);
  glClear(buffers);
}
