void WebContentsImpl::DidChangeName(RenderFrameHost* render_frame_host,
                                    const std::string& name) {
  for (auto& observer : observers_)
    observer.FrameNameChanged(render_frame_host, name);
}
