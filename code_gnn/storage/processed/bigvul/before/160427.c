void CreateMediaPlayerRenderer(int process_id,
                               int routing_id,
                               RenderFrameHostDelegate* delegate,
                               media::mojom::RendererRequest request) {
  std::unique_ptr<MediaPlayerRenderer> renderer =
      std::make_unique<MediaPlayerRenderer>(process_id, routing_id,
                                            delegate->GetAsWebContents());

  media::MojoRendererService::InitiateSurfaceRequestCB surface_request_cb =
      base::Bind(&MediaPlayerRenderer::InitiateScopedSurfaceRequest,
                 base::Unretained(renderer.get()));

  media::MojoRendererService::Create(
      nullptr,   
      nullptr,   
      nullptr,   
      std::move(renderer), surface_request_cb, std::move(request));
}
