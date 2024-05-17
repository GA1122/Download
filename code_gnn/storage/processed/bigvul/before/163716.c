void WebContentsImpl::RunBeforeUnloadConfirm(
    RenderFrameHost* render_frame_host,
    bool is_reload,
    IPC::Message* reply_msg) {
  if (IsFullscreenForCurrentTab())
    ExitFullscreen(true);

  RenderFrameHostImpl* rfhi =
      static_cast<RenderFrameHostImpl*>(render_frame_host);
  if (delegate_)
    delegate_->WillRunBeforeUnloadConfirm();

  base::Callback<void(bool, bool, const base::string16&)> callback =
      base::Bind(&WebContentsImpl::OnDialogClosed, base::Unretained(this),
                 render_frame_host->GetProcess()->GetID(),
                 render_frame_host->GetRoutingID(), reply_msg);

  bool suppress_this_message = !rfhi->is_active() ||
                               ShowingInterstitialPage() || !delegate_ ||
                               delegate_->ShouldSuppressDialogs(this);
  if (delegate_)
    dialog_manager_ = delegate_->GetJavaScriptDialogManager(this);

  std::vector<protocol::PageHandler*> page_handlers =
      protocol::PageHandler::EnabledForWebContents(this);

  if (suppress_this_message || (!dialog_manager_ && !page_handlers.size())) {
    callback.Run(false, true, base::string16());
    return;
  }

  is_showing_before_unload_dialog_ = true;

  scoped_refptr<CloseDialogCallbackWrapper> wrapper =
      new CloseDialogCallbackWrapper(callback);
  callback = base::Bind(&CloseDialogCallbackWrapper::Run, wrapper);

  GURL frame_url = rfhi->GetLastCommittedURL();
  for (auto* handler : page_handlers) {
    handler->DidRunBeforeUnloadConfirm(frame_url, base::Bind(callback, false));
  }

  if (dialog_manager_) {
    dialog_manager_->RunBeforeUnloadDialog(this, is_reload,
                                           base::Bind(callback, false));
  }
}
