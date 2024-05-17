void WebContentsImpl::RunJavaScriptDialog(RenderFrameHost* render_frame_host,
                                          const base::string16& message,
                                          const base::string16& default_prompt,
                                          const GURL& frame_url,
                                          JavaScriptDialogType dialog_type,
                                          IPC::Message* reply_msg) {
  if (IsFullscreenForCurrentTab())
    ExitFullscreen(true);

  base::Callback<void(bool, bool, const base::string16&)> callback =
      base::Bind(&WebContentsImpl::OnDialogClosed, base::Unretained(this),
                 render_frame_host->GetProcess()->GetID(),
                 render_frame_host->GetRoutingID(), reply_msg);

  bool suppress_this_message = ShowingInterstitialPage() || !delegate_ ||
                               delegate_->ShouldSuppressDialogs(this);
  if (delegate_)
    dialog_manager_ = delegate_->GetJavaScriptDialogManager(this);

  std::vector<protocol::PageHandler*> page_handlers =
      protocol::PageHandler::EnabledForWebContents(this);

  if (suppress_this_message || (!dialog_manager_ && !page_handlers.size())) {
    callback.Run(true, false, base::string16());
    return;
  }

  scoped_refptr<CloseDialogCallbackWrapper> wrapper =
      new CloseDialogCallbackWrapper(callback);
  callback = base::Bind(&CloseDialogCallbackWrapper::Run, wrapper);

  is_showing_javascript_dialog_ = true;

  for (auto* handler : page_handlers) {
    handler->DidRunJavaScriptDialog(frame_url, message, default_prompt,
                                    dialog_type, base::Bind(callback, false));
  }

  if (dialog_manager_) {
    dialog_manager_->RunJavaScriptDialog(
        this, frame_url, dialog_type, message, default_prompt,
        base::Bind(callback, false), &suppress_this_message);
  }

  if (suppress_this_message) {
    callback.Run(true, false, base::string16());
  }
}
