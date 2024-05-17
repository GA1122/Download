void RenderThreadImpl::CreateView(mojom::CreateViewParamsPtr params) {
  CompositorDependencies* compositor_deps = this;
  is_scroll_animator_enabled_ = params->web_preferences.enable_scroll_animator;
  RenderViewImpl::Create(compositor_deps, std::move(params),
                         RenderWidget::ShowCallback());
}
