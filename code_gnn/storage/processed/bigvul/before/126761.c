gfx::Rect BrowserView::GetClientAreaBounds() const {
  gfx::Rect container_bounds = contents_->bounds();
  gfx::Point container_origin = container_bounds.origin();
  ConvertPointToTarget(this, parent(), &container_origin);
  container_bounds.set_origin(container_origin);
  return container_bounds;
}
