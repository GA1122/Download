void ForwardToJavaFrameRegistry(
    mojo::InterfaceRequest<Interface> request,
    content::RenderFrameHost* render_frame_host) {
  render_frame_host->GetJavaInterfaces()->GetInterface(std::move(request));
}
