void WebContentsImpl::DidChangeName(RenderFrameHost* render_frame_host,
                                    const std::string& name) {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    FrameNameChanged(render_frame_host, name));
}
