void WebContentsImpl::DidNavigateAnyFramePostCommit(
    RenderFrameHostImpl* render_frame_host,
    const LoadCommittedDetails& details,
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params) {
  has_accessed_initial_document_ = false;

  if (!details.is_in_page)
    CancelActiveAndPendingDialogs();

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidNavigateAnyFrame(render_frame_host, details, params));
}
