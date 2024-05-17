void HTMLCanvasElement::DiscardResourceProvider() {
  canvas2d_bridge_.reset();
  CanvasResourceHost::DiscardResourceProvider();
  dirty_rect_ = FloatRect();
}
