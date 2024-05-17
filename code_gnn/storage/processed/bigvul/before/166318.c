    CreateObserverForSession(int render_process_id,
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

  auto& observer = contents_observers_[web_contents];
  if (!observer)
    observer = std::make_unique<ContentsObserver>(web_contents, this);

  observer->AddObservedFrame(render_frame_host, session_id);
}
