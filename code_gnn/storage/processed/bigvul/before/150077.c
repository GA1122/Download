  ViewportAnchor(LayerImpl* inner_scroll, LayerImpl* outer_scroll)
      : inner_(inner_scroll), outer_(outer_scroll) {
    viewport_in_content_coordinates_ = inner_->CurrentScrollOffset();

    if (outer_)
      viewport_in_content_coordinates_ += outer_->CurrentScrollOffset();
  }
