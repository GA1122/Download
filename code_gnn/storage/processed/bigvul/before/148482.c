void WebContentsImpl::OnFrameRemoved(RenderFrameHost* render_frame_host) {
  for (auto& observer : observers_)
    observer.FrameDeleted(render_frame_host);
}
