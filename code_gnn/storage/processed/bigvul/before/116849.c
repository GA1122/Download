void RenderThreadImpl::OnCreateNewView(const ViewMsg_New_Params& params) {
  EnsureWebKitInitialized();
  RenderViewImpl::Create(
      params.parent_window,
      MSG_ROUTING_NONE,
      params.renderer_preferences,
      params.web_preferences,
      new SharedRenderViewCounter(0),
      params.view_id,
      params.surface_id,
      params.session_storage_namespace_id,
      params.frame_name,
      params.next_page_id,
      params.screen_info,
      params.guest);
}
