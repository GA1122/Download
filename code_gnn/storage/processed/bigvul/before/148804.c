void InterstitialPageImpl::RenderFrameCreated(
    RenderFrameHost* render_frame_host) {
  controller_->delegate()->RenderFrameForInterstitialPageCreated(
      render_frame_host);
}
