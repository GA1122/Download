bool HTMLCanvasElement::IsAnimated2d() const {
  return Is2d() && canvas2d_bridge_ &&
         canvas2d_bridge_->WasDrawnToAfterSnapshot();
}
