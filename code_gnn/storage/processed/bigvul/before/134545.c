void WebContentsViewAura::SetOverscrollControllerEnabled(bool enabled) {
  RenderViewHostImpl* host = static_cast<RenderViewHostImpl*>(
      web_contents_->GetRenderViewHost());
  if (host) {
    host->SetOverscrollControllerEnabled(enabled);
    if (enabled)
      host->overscroll_controller()->set_delegate(this);
  }

  if (!enabled)
    navigation_overlay_.reset();
  else if (!navigation_overlay_)
    navigation_overlay_.reset(new OverscrollNavigationOverlay(web_contents_));
}
