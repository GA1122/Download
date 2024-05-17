void ExtensionWebContentsObserver::InitializeFrameHelper(
    content::RenderFrameHost* render_frame_host) {
  if (render_frame_host->IsRenderFrameLive())
    InitializeRenderFrame(render_frame_host);
}
