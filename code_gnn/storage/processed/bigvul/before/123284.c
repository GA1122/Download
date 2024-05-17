void RenderWidgetHostViewGtk::DoSharedInit() {
  view_.Own(RenderWidgetHostViewGtkWidget::CreateNewWidget(this));
  im_context_.reset(new GtkIMContextWrapper(this));
  plugin_container_manager_.set_host_widget(view_.get());
  key_bindings_handler_.reset(new GtkKeyBindingsHandler(view_.get()));
}
