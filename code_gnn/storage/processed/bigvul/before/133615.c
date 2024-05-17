void WebContentsImpl::RenderFrameCreated(RenderFrameHost* render_frame_host) {
  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    RenderFrameCreated(render_frame_host));
}
