void WebContentsImpl::DidNavigateAnyFramePostCommit(
    RenderFrameHostImpl* render_frame_host,
    const LoadCommittedDetails& details,
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params) {
  has_accessed_initial_document_ = false;

  if (!details.is_same_document)
    CancelActiveAndPendingDialogs();

  if (params.gesture == NavigationGestureUser && dialog_manager_) {
    dialog_manager_->CancelDialogs(this,  true);
  }
}
