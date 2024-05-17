void BrowserPluginGuest::SetVisibility(bool embedder_visible, bool visible) {
  visible_ = visible;
  if (embedder_visible && visible)
    web_contents()->WasShown();
  else
    web_contents()->WasHidden();
}
