void WebContentsImpl::RenderFrameDeleted(RenderFrameHost* render_frame_host) {
  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    RenderFrameDeleted(render_frame_host));
}
