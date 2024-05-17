void RenderWidgetHostImpl::SetWidgetInputHandler(
    mojom::WidgetInputHandlerAssociatedPtr widget_input_handler,
    mojom::WidgetInputHandlerHostRequest host_request) {
  if (base::FeatureList::IsEnabled(features::kMojoInputMessages)) {
    associated_widget_input_handler_ = std::move(widget_input_handler);
    input_router_->BindHost(std::move(host_request), true);
  }
}
