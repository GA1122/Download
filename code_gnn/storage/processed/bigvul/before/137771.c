void HeadlessWebContentsImpl::RenderFrameCreated(
    content::RenderFrameHost* render_frame_host) {
  for (const MojoService& service : mojo_services_) {
    registry_.AddInterface(
        service.service_name,
        base::Bind(&HeadlessWebContentsImpl::CreateMojoService,
                   base::Unretained(this), service.service_factory),
        browser()->BrowserMainThread());
  }

  browser_context_->SetDevToolsFrameToken(
      render_frame_host->GetProcess()->GetID(),
      render_frame_host->GetRoutingID(),
      render_frame_host->GetDevToolsFrameToken(),
      render_frame_host->GetFrameTreeNodeId());

  if (headless_tab_socket_)
    headless_tab_socket_->RenderFrameCreated(render_frame_host);
}
