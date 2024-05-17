Response PageHandler::Disable() {
  enabled_ = false;
  screencast_enabled_ = false;

  if (video_consumer_)
    video_consumer_->StopCapture();

  if (!pending_dialog_.is_null()) {
    WebContentsImpl* web_contents = GetWebContents();
    bool has_dialog_manager =
        web_contents && web_contents->GetDelegate() &&
        web_contents->GetDelegate()->GetJavaScriptDialogManager(web_contents);
    if (!has_dialog_manager)
      std::move(pending_dialog_).Run(false, base::string16());
    pending_dialog_.Reset();
  }

  download_manager_delegate_ = nullptr;
  navigate_callbacks_.clear();
  return Response::FallThrough();
}
