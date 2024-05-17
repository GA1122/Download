void WebContentsImpl::RenderFrameForInterstitialPageCreated(
    RenderFrameHost* render_frame_host) {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    RenderFrameForInterstitialPageCreated(render_frame_host));
}
