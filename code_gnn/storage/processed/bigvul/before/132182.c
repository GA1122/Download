RenderFrameImpl* RenderFrameImpl::Create(RenderViewImpl* render_view,
                                         int32 routing_id) {
  DCHECK(routing_id != MSG_ROUTING_NONE);
  CreateParams params(render_view, routing_id);

  if (g_create_render_frame_impl)
    return g_create_render_frame_impl(params);
  else
    return new RenderFrameImpl(params);
}
