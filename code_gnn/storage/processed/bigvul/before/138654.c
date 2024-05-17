void CreateMediaPlayerRenderer(
    content::RenderFrameHost* render_frame_host,
    mojo::InterfaceRequest<media::mojom::Renderer> request) {
  std::unique_ptr<MediaPlayerRenderer> renderer =
      base::MakeUnique<MediaPlayerRenderer>(render_frame_host);

  media::MojoRendererService::InitiateSurfaceRequestCB surface_request_cb =
      base::Bind(&MediaPlayerRenderer::InitiateScopedSurfaceRequest,
                 base::Unretained(renderer.get()));

  media::MojoRendererService::Create(
      nullptr,   
      nullptr,   
      nullptr,   
      std::move(renderer), surface_request_cb, std::move(request));
}
