void BrowserViewRenderer::UpdateCompositorIsActive() {
  if (compositor_) {
    if (disable_page_visibility_)
      compositor_->SetIsActive(!is_paused_ &&
                               (!attached_to_window_ || window_visible_));
    else
      compositor_->SetIsActive(IsClientVisible());
  }
}
