RenderFrameImpl::CreateParams::CreateParams(
    RenderViewImpl* render_view,
    int32_t routing_id,
    service_manager::mojom::InterfaceProviderPtr interface_provider,
    const base::UnguessableToken& devtools_frame_token)
    : render_view(render_view),
      routing_id(routing_id),
      interface_provider(std::move(interface_provider)),
      devtools_frame_token(devtools_frame_token) {}
