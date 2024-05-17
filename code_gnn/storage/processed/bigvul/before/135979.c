void ChromeExtensionWebContentsObserver::InitializeRenderFrame(
    content::RenderFrameHost* render_frame_host) {
  ExtensionWebContentsObserver::InitializeRenderFrame(render_frame_host);
  WindowController* controller = dispatcher()->GetExtensionWindowController();
  if (controller) {
    render_frame_host->Send(new ExtensionMsg_UpdateBrowserWindowId(
        render_frame_host->GetRoutingID(), controller->GetWindowId()));
  }
}
