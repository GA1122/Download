void NotifyResourceSchedulerOfNavigation(
    int render_process_id,
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params) {
  if (!ui::PageTransitionIsMainFrame(params.transition))
    return;

  base::PostTaskWithTraits(
      FROM_HERE, {BrowserThread::IO},
      base::BindOnce(&ResourceSchedulerFilter::OnDidCommitMainframeNavigation,
                     render_process_id, params.render_view_routing_id));
}
