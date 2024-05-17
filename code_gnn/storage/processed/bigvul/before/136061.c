void ExtensionWebContentsObserver::RenderFrameCreated(
    content::RenderFrameHost* render_frame_host) {
  InitializeRenderFrame(render_frame_host);
}
