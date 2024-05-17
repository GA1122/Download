bool DrawingBuffer::WantExplicitResolve() {
  return anti_aliasing_mode_ == kMSAAExplicitResolve;
}
