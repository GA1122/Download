void LogResourceRequestTimeOnUI(
    base::TimeTicks timestamp,
    int render_process_id,
    int render_frame_id,
    const GURL& url) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  RenderFrameHostImpl* host =
      RenderFrameHostImpl::FromID(render_process_id, render_frame_id);
  if (host != nullptr) {
    DCHECK(host->frame_tree_node()->IsMainFrame());
    host->frame_tree_node()->navigator()->LogResourceRequestTime(
        timestamp, url);
  }
}
