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
    bool is_renderer_created,
    bool swapped_out,
    int32 next_page_id,
    const WebKit::WebScreenInfo& screen_info,
    content::old::GuestToEmbedderChannel* guest_to_embedder_channel,
    AccessibilityMode accessibility_mode) {
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
      is_renderer_created,
      swapped_out,
      next_page_id,
      screen_info,
      guest_to_embedder_channel,
      accessibility_mode);
}
