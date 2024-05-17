void WebContentsImpl::RenderFrameCreated(RenderFrameHost* render_frame_host) {
  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    RenderFrameCreated(render_frame_host));
  SetAccessibilityModeOnFrame(accessibility_mode_, render_frame_host);
}
