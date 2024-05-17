WebUIBindings* RenderViewImpl::GetWebUIBindings() {
  if (!web_ui_bindings_.get()) {
    web_ui_bindings_.reset(new WebUIBindings(
        static_cast<content::RenderView*>(this), routing_id_));
  }
  return web_ui_bindings_.get();
}
