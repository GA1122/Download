void NotifyResourceSchedulerOfNavigation(
    int render_process_id,
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params) {
  if (!ui::PageTransitionIsMainFrame(params.transition) ||
      params.was_within_same_document) {
    return;
  }

  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE,
      base::Bind(&ResourceSchedulerFilter::OnDidCommitMainframeNavigation,
                 render_process_id, params.render_view_routing_id));
}
