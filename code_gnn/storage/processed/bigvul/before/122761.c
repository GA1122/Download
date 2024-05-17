void BrowserPluginGuest::RenderViewReady() {
  Send(new ViewMsg_SetFocus(routing_id(), focused_));
  bool embedder_visible =
      embedder_web_contents_->GetBrowserPluginEmbedder()->visible();
  SetVisibility(embedder_visible, visible());
  if (auto_size_enabled_) {
    web_contents()->GetRenderViewHost()->EnableAutoResize(
        min_auto_size_, max_auto_size_);
  } else {
    web_contents()->GetRenderViewHost()->DisableAutoResize(damage_view_size_);
  }
}
