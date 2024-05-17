void VRDisplay::UpdateLayerBounds() {
  if (!display_)
    return;

  device::mojom::blink::VRLayerBoundsPtr left_bounds =
      device::mojom::blink::VRLayerBounds::New();
  device::mojom::blink::VRLayerBoundsPtr right_bounds =
      device::mojom::blink::VRLayerBounds::New();

  if (layer_.leftBounds().size() == 4) {
    left_bounds->left = layer_.leftBounds()[0];
    left_bounds->top = layer_.leftBounds()[1];
    left_bounds->width = layer_.leftBounds()[2];
    left_bounds->height = layer_.leftBounds()[3];
  } else {
    left_bounds->left = 0.0f;
    left_bounds->top = 0.0f;
    left_bounds->width = 0.5f;
    left_bounds->height = 1.0f;
    layer_.setLeftBounds({0.0f, 0.0f, 0.5f, 1.0f});
  }

  if (layer_.rightBounds().size() == 4) {
    right_bounds->left = layer_.rightBounds()[0];
    right_bounds->top = layer_.rightBounds()[1];
    right_bounds->width = layer_.rightBounds()[2];
    right_bounds->height = layer_.rightBounds()[3];
  } else {
    right_bounds->left = 0.5f;
    right_bounds->top = 0.0f;
    right_bounds->width = 0.5f;
    right_bounds->height = 1.0f;
    layer_.setRightBounds({0.5f, 0.0f, 0.5f, 1.0f});
  }

  display_->UpdateLayerBounds(vr_frame_id_, std::move(left_bounds),
                              std::move(right_bounds), source_width_,
                              source_height_);
}
