void RenderThread::OnCreateNewView(gfx::NativeViewId parent_hwnd,
                                   const RendererPreferences& renderer_prefs,
                                   const WebPreferences& webkit_prefs,
                                   int32 view_id) {
  EnsureWebKitInitialized();
  RenderView::Create(
      this, parent_hwnd, MSG_ROUTING_NONE, renderer_prefs,
      webkit_prefs, new SharedRenderViewCounter(0), view_id);
}
