bool WebContentsImpl::OnMessageReceived(RenderFrameHost* render_frame_host,
                                        const IPC::Message& message) {
  return OnMessageReceived(NULL, render_frame_host, message);
}
