bool PanelBrowserView::IsAnimatingBounds() const {
  return bounds_animator_.get() && bounds_animator_->is_animating();
}
