RenderViewImpl* RenderViewImpl::Create(
    gfx::NativeViewId parent_hwnd,
    int32 opener_id,
    const content::RendererPreferences& renderer_prefs,
    const WebPreferences& webkit_prefs,
    SharedRenderViewCounter* counter,
    int32 routing_id,
    int32 surface_id,
    int64 session_storage_namespace_id,
    const string16& frame_name,
    int32 next_page_id,
    const WebKit::WebScreenInfo& screen_info) {
  DCHECK(routing_id != MSG_ROUTING_NONE);
  return new RenderViewImpl(
      parent_hwnd,
      opener_id,
      renderer_prefs,
      webkit_prefs,
      counter,
      routing_id,
      surface_id,
      session_storage_namespace_id,
      frame_name,
      next_page_id,
      screen_info);
}
