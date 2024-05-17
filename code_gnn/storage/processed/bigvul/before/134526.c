void WebContentsViewAura::OnWindowDestroying() {
  navigation_overlay_.reset();
  overscroll_window_.reset();
}
