IntSize WebGLRenderingContextBase::ClampedCanvasSize() const {
  int width = Host()->Size().Width();
  int height = Host()->Size().Height();
  return IntSize(Clamp(width, 1, max_viewport_dims_[0]),
                 Clamp(height, 1, max_viewport_dims_[1]));
}
