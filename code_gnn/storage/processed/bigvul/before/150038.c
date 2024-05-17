void LayerTreeHostImpl::SetExternalTilePriorityConstraints(
    const gfx::Rect& viewport_rect,
    const gfx::Transform& transform) {
  gfx::Rect viewport_rect_for_tile_priority_in_view_space;
  gfx::Transform screen_to_view(gfx::Transform::kSkipInitialization);
  if (transform.GetInverse(&screen_to_view)) {
    viewport_rect_for_tile_priority_in_view_space =
        MathUtil::ProjectEnclosingClippedRect(screen_to_view, viewport_rect);
  }

  const bool tile_priority_params_changed =
      viewport_rect_for_tile_priority_ !=
      viewport_rect_for_tile_priority_in_view_space;

  viewport_rect_for_tile_priority_ =
      viewport_rect_for_tile_priority_in_view_space;

  if (tile_priority_params_changed) {
    active_tree_->set_needs_update_draw_properties();
    if (pending_tree_)
      pending_tree_->set_needs_update_draw_properties();

    SetFullViewportDamage();
    SetNeedsRedraw();
  }
}
