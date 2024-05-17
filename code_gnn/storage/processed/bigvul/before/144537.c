WebContentsImpl* WebContentsImpl::GetOuterWebContents() {
  if (BrowserPluginGuestMode::UseCrossProcessFramesForGuests()) {
    if (node_)
      return node_->outer_web_contents();
  } else {
    if (GetBrowserPluginGuest())
      return GetBrowserPluginGuest()->embedder_web_contents();
  }
  return nullptr;
}
