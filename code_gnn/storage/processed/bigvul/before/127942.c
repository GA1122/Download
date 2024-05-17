bool BrowserViewRenderer::IsClientVisible() const {
  if (disable_page_visibility_)
    return !is_paused_;

  return !is_paused_ && IsVisible();
}
