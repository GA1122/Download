void BrowserViewRenderer::UpdateCompositorIsActive() {
  if (compositor_)
    compositor_->SetIsActive(!is_paused_ &&
                             (!attached_to_window_ || window_visible_));
}
