void RenderThread::OnCreateNewView(const ViewMsg_New_Params& params) {
  EnsureWebKitInitialized();
  RenderView::Create(
      this,
      params.parent_window,
      MSG_ROUTING_NONE,
      params.renderer_preferences,
      params.web_preferences,
      new SharedRenderViewCounter(0),
      params.view_id,
      params.session_storage_namespace_id,
      params.frame_name);
}
