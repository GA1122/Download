RenderViewImpl* RenderViewImpl::Create(
    CompositorDependencies* compositor_deps,
    mojom::CreateViewParamsPtr params,
    RenderWidget::ShowCallback show_callback,
    scoped_refptr<base::SingleThreadTaskRunner> task_runner) {
  DCHECK(params->view_id != MSG_ROUTING_NONE);
  DCHECK(params->main_frame_widget_routing_id != MSG_ROUTING_NONE);
  RenderViewImpl* render_view;
  if (g_create_render_view_impl)
    render_view = g_create_render_view_impl(compositor_deps, *params);
  else
    render_view = new RenderViewImpl(compositor_deps, *params);

  render_view->Initialize(std::move(params), std::move(show_callback),
                          std::move(task_runner));
  return render_view;
}
