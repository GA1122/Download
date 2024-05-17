RenderFrameImpl* RenderFrameImpl::Create(
    RenderViewImpl* render_view,
    int32_t routing_id,
    service_manager::mojom::InterfaceProviderPtr interface_provider,
    const base::UnguessableToken& devtools_frame_token) {
  DCHECK(routing_id != MSG_ROUTING_NONE);
  CreateParams params(render_view, routing_id, std::move(interface_provider),
                      devtools_frame_token);

  if (g_create_render_frame_impl)
    return g_create_render_frame_impl(std::move(params));
  else
    return new RenderFrameImpl(std::move(params));
}
