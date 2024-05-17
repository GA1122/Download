void RenderView::OnSetWebUIProperty(const std::string& name,
                                    const std::string& value) {
  DCHECK(BindingsPolicy::is_web_ui_enabled(enabled_bindings_));
  GetWebUIBindings()->SetProperty(name, value);
}
