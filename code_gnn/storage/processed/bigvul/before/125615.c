RenderViewHostImpl* RenderViewHostImpl::FromID(int render_process_id,
                                               int render_view_id) {
  return static_cast<RenderViewHostImpl*>(
      RenderViewHost::FromID(render_process_id, render_view_id));
}
