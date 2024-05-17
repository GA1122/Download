void WebContentsViewAura::OnWindowTargetVisibilityChanged(bool visible) {
  if (visible)
    web_contents_->WasShown();
  else
    web_contents_->WasHidden();
}
