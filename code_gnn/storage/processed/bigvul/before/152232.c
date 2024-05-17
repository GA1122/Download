RenderFrameImpl::CreateParams::CreateParams(
    RenderViewImpl* render_view,
    int32_t routing_id,
    service_manager::mojom::InterfaceProviderPtr interface_provider,
    blink::mojom::DocumentInterfaceBrokerPtr document_interface_broker_content,
    const base::UnguessableToken& devtools_frame_token)
    : render_view(render_view),
      routing_id(routing_id),
      interface_provider(std::move(interface_provider)),
      document_interface_broker_content(
          std::move(document_interface_broker_content)),
      devtools_frame_token(devtools_frame_token) {}
