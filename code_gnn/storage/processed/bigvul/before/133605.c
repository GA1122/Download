bool WebContentsImpl::OnMessageReceived(RenderViewHost* render_view_host,
                                        const IPC::Message& message) {
  return OnMessageReceived(render_view_host, NULL, message);
}
