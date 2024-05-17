void WebContentsViewAura::SetupOverlayWindowForTesting() {
  if (navigation_overlay_)
    navigation_overlay_->SetupForTesting();
}
