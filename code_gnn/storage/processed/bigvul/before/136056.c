void ExtensionWebContentsObserver::InitializeRenderFrame(
    content::RenderFrameHost* render_frame_host) {
  DCHECK(render_frame_host);
  DCHECK(render_frame_host->IsRenderFrameLive());

  render_frame_host->Send(new ExtensionMsg_NotifyRenderViewType(
      render_frame_host->GetRoutingID(), GetViewType(web_contents())));

  const Extension* frame_extension = GetExtensionFromFrame(render_frame_host);
  if (frame_extension) {
    ExtensionsBrowserClient::Get()->RegisterMojoServices(render_frame_host,
                                                         frame_extension);
    ProcessManager::Get(browser_context_)
        ->RegisterRenderFrameHost(web_contents(), render_frame_host,
                                  frame_extension);
  }
}
