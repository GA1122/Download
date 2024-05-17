void LockScreenMediaControlsView::UpdateDrag(
    const gfx::Point& location_in_screen) {
  is_in_drag_ = true;
  int drag_delta = location_in_screen.y() - initial_drag_point_.y();

  if (contents_view_->bounds().bottom() + drag_delta >=
      GetLocalBounds().bottom()) {
    return;
  }

  gfx::Transform transform;
  transform.Translate(0, drag_delta);
  contents_view_->layer()->SetTransform(transform);
  UpdateOpacity();
}
