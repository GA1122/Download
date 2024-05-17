gfx::Rect PictureLayerImpl::GetViewportForTilePriorityInContentSpace() const {
  gfx::Rect visible_rect_in_content_space = visible_rect_for_tile_priority_;
  gfx::Rect viewport_rect_for_tile_priority =
      layer_tree_impl()->ViewportRectForTilePriority();

  if (visible_rect_in_content_space.IsEmpty() ||
      layer_tree_impl()->DeviceViewport() != viewport_rect_for_tile_priority) {
    gfx::Transform view_to_layer(gfx::Transform::kSkipInitialization);
    if (screen_space_transform().GetInverse(&view_to_layer)) {
      visible_rect_in_content_space =
          gfx::ToEnclosingRect(MathUtil::ProjectClippedRect(
              view_to_layer, viewport_rect_for_tile_priority));
    }
  }
  return visible_rect_in_content_space;
}
