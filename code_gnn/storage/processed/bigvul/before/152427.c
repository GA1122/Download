void RenderFrameImpl::OnCopyToFindPboard() {
  if (frame_->HasSelection()) {
    if (!clipboard_host_) {
      auto* platform = RenderThreadImpl::current_blink_platform_impl();
      platform->GetConnector()->BindInterface(platform->GetBrowserServiceName(),
                                              &clipboard_host_);
      clipboard_host_.set_connection_error_handler(base::BindOnce(
          &RenderFrameImpl::OnClipboardHostError, base::Unretained(this)));
    }
    base::string16 selection = frame_->SelectionAsText().Utf16();
    clipboard_host_->WriteStringToFindPboard(selection);
  }
}
