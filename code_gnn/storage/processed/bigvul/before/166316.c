    AddObservedFrame(RenderFrameHost* render_frame_host, int session_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  observed_frames_.emplace(render_frame_host, session_id);
}
