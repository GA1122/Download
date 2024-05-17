void ExtensionWebContentsObserver::RenderFrameDeleted(
    content::RenderFrameHost* render_frame_host) {
  ProcessManager::Get(browser_context_)
      ->UnregisterRenderFrameHost(render_frame_host);
}
