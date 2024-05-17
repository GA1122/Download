void BrowserPluginGuest::SetSize(
    const BrowserPluginHostMsg_AutoSize_Params& auto_size_params,
    const BrowserPluginHostMsg_ResizeGuest_Params& resize_guest_params) {
  bool old_auto_size_enabled = auto_size_enabled_;
  gfx::Size old_max_size = max_auto_size_;
  gfx::Size old_min_size = min_auto_size_;
  auto_size_enabled_ = auto_size_params.enable;
  max_auto_size_ = auto_size_params.max_size;
  min_auto_size_ = auto_size_params.min_size;
  if (auto_size_enabled_ && (!old_auto_size_enabled ||
                             (old_max_size != max_auto_size_) ||
                             (old_min_size != min_auto_size_))) {
    web_contents()->GetRenderViewHost()->EnableAutoResize(
        min_auto_size_, max_auto_size_);
    web_contents()->GetRenderViewHost()->Send(new ViewMsg_Repaint(
        web_contents()->GetRenderViewHost()->GetRoutingID(),
        max_auto_size_));
  } else if (!auto_size_enabled_ && old_auto_size_enabled) {
    web_contents()->GetRenderViewHost()->DisableAutoResize(
        resize_guest_params.view_size);
  }
  Resize(embedder_web_contents_->GetRenderViewHost(), resize_guest_params);
}
