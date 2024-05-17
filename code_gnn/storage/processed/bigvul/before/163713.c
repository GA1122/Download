void WebContentsImpl::OnInterfaceRequest(
    RenderFrameHost* render_frame_host,
    const std::string& interface_name,
    mojo::ScopedMessagePipeHandle* interface_pipe) {
  for (auto& observer : observers_) {
    observer.OnInterfaceRequestFromFrame(render_frame_host, interface_name,
                                         interface_pipe);
    if (!interface_pipe->is_valid())
      break;
  }
}
