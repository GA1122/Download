SkColor Layer::SafeOpaqueBackgroundColor() const {
  SkColor color = background_color();
  if (SkColorGetA(color) == 255 && !contents_opaque()) {
    color = SK_ColorTRANSPARENT;
  } else if (SkColorGetA(color) != 255 && contents_opaque()) {
    for (const Layer* layer = parent(); layer;
         layer = layer->parent()) {
      color = layer->background_color();
      if (SkColorGetA(color) == 255)
        break;
    }
    if (SkColorGetA(color) != 255)
      color = layer_tree_host_->background_color();
    if (SkColorGetA(color) != 255)
      color = SkColorSetA(color, 255);
  }
  return color;
}
