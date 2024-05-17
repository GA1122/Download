WebContents* WebContentsImpl::FromRenderFrameHostID(int render_process_host_id,
                                                    int render_frame_host_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  RenderFrameHost* render_frame_host =
      RenderFrameHost::FromID(render_process_host_id, render_frame_host_id);
  if (!render_frame_host)
    return nullptr;

  return WebContents::FromRenderFrameHost(render_frame_host);
}
