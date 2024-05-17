RenderViewImpl::RenderViewImpl(CompositorDependencies* compositor_deps,
                               const mojom::CreateViewParams& params)
    : RenderWidget(params.main_frame_widget_routing_id,
                   compositor_deps,
                   WidgetType::kFrame,
                   params.visual_properties.screen_info,
                   params.visual_properties.display_mode,
                   params.swapped_out,
                   params.hidden,
                   params.never_visible),
      routing_id_(params.view_id),
      renderer_wide_named_frame_lookup_(
          params.renderer_wide_named_frame_lookup),
      webkit_preferences_(params.web_preferences),
      session_storage_namespace_id_(params.session_storage_namespace_id),
      weak_ptr_factory_(this) {
  GetWidget()->set_owner_delegate(this);
  RenderThread::Get()->AddRoute(routing_id_, this);
}
