void RenderFrameImpl::InstallCreateHook(
    CreateRenderFrameImplFunction create_frame,
    CreateRenderWidgetForChildLocalRootFunction create_widget,
    RenderWidgetForChildLocalRootInitializedCallback widget_initialized) {
  DCHECK(!g_create_render_frame_impl);
  DCHECK(!g_create_render_widget);
  DCHECK(!g_render_widget_initialized);
  g_create_render_frame_impl = create_frame;
  g_create_render_widget = create_widget;
  g_render_widget_initialized = widget_initialized;
}
