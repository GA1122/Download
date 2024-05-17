void WebContentsImpl::RunJavaScriptMessage(
    RenderFrameHost* render_frame_host,
    const base::string16& message,
    const base::string16& default_prompt,
    const GURL& frame_url,
    JavaScriptMessageType javascript_message_type,
    IPC::Message* reply_msg) {
  bool suppress_this_message =
      static_cast<RenderFrameHostImpl*>(render_frame_host)->is_swapped_out() ||
      ShowingInterstitialPage() || !delegate_ ||
      delegate_->ShouldSuppressDialogs(this) ||
      !delegate_->GetJavaScriptDialogManager(this);

  if (!suppress_this_message) {
    std::string accept_lang = GetContentClient()->browser()->
      GetAcceptLangs(GetBrowserContext());
    dialog_manager_ = delegate_->GetJavaScriptDialogManager(this);
    dialog_manager_->RunJavaScriptDialog(
        this, frame_url, accept_lang, javascript_message_type, message,
        default_prompt,
        base::Bind(&WebContentsImpl::OnDialogClosed, base::Unretained(this),
                   render_frame_host->GetProcess()->GetID(),
                   render_frame_host->GetRoutingID(), reply_msg, false),
        &suppress_this_message);
  }

  if (suppress_this_message) {
    OnDialogClosed(render_frame_host->GetProcess()->GetID(),
                   render_frame_host->GetRoutingID(), reply_msg,
                   true, false, base::string16());
  }

}
