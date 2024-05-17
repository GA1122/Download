RenderViewImpl* RenderViewImpl::Create(
    CompositorDependencies* compositor_deps,
    mojom::CreateViewParamsPtr params,
    const RenderWidget::ShowCallback& show_callback,
    scoped_refptr<base::SingleThreadTaskRunner> task_runner) {
  DCHECK(params->view_id != MSG_ROUTING_NONE);
  RenderViewImpl* render_view;
  if (g_create_render_view_impl)
    render_view = g_create_render_view_impl(compositor_deps, *params);
  else
    render_view = new RenderViewImpl(compositor_deps, *params, task_runner);

  render_view->Initialize(std::move(params), show_callback);
  return render_view;
}
