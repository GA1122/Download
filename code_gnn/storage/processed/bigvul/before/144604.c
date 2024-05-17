void WebContentsImpl::OnFrameRemoved(RenderFrameHost* render_frame_host) {
  FOR_EACH_OBSERVER(
      WebContentsObserver, observers_, FrameDeleted(render_frame_host));
}
