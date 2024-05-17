void NotifyResourceSchedulerOfNavigation(
    int render_process_id,
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params) {
  if (!ui::PageTransitionIsMainFrame(params.transition))
    return;

  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE,
      base::BindOnce(&ResourceSchedulerFilter::OnDidCommitMainframeNavigation,
                     render_process_id, params.render_view_routing_id));
}
