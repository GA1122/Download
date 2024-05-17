void WebContentsImpl::RenderFrameForInterstitialPageCreated(
    RenderFrameHost* render_frame_host) {
  for (auto& observer : observers_)
    observer.RenderFrameForInterstitialPageCreated(render_frame_host);
}
