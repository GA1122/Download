    RemoveObserverForSession(int render_process_id,
                             int render_frame_id,
                             int session_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  RenderFrameHost* render_frame_host =
      RenderFrameHost::FromID(render_process_id, render_frame_id);
  if (!render_frame_host)
    return;

  WebContents* web_contents =
      WebContents::FromRenderFrameHost(render_frame_host);
  if (!web_contents)
    return;

  auto it = contents_observers_.find(web_contents);
  if (it == contents_observers_.end())
    return;

  it->second->RemoveObservedFrame(render_frame_host, session_id);
}
