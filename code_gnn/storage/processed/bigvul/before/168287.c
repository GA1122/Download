gfx::Rect BrowserView::GetRestoredBounds() const {
  gfx::Rect bounds;
  ui::WindowShowState state;
  frame_->GetWindowPlacement(&bounds, &state);
  return bounds;
}
