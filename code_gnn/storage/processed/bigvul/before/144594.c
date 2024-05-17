void WebContentsImpl::OnBrowserPluginMessage(RenderFrameHost* render_frame_host,
                                             const IPC::Message& message) {
  CHECK(!browser_plugin_embedder_.get());
  CreateBrowserPluginEmbedderIfNecessary();
  browser_plugin_embedder_->OnMessageReceived(message, render_frame_host);
}
