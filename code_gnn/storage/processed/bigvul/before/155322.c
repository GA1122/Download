void ForwardToJavaWebContentsRegistry(
    mojo::InterfaceRequest<Interface> request,
    content::RenderFrameHost* render_frame_host) {
  content::WebContents* contents =
      content::WebContents::FromRenderFrameHost(render_frame_host);
  if (contents)
    contents->GetJavaInterfaces()->GetInterface(std::move(request));
}
