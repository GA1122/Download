mojom::WidgetInputHandler* RenderWidgetHostImpl::GetWidgetInputHandler() {
  if (associated_widget_input_handler_)
    return associated_widget_input_handler_.get();
  if (widget_input_handler_)
    return widget_input_handler_.get();
  return g_unbound_input_handler.Pointer();
}
