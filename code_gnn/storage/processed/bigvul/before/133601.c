void WebContentsImpl::OnFrameRemoved(
    RenderViewHostImpl* render_view_host,
    int64 frame_id) {
   FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                     FrameDetached(render_view_host, frame_id));
}
