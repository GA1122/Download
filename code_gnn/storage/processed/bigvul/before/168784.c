void RenderWidgetHostImpl::SetWidget(mojom::WidgetPtr widget) {
  if (widget && base::FeatureList::IsEnabled(features::kMojoInputMessages)) {
    if (widget_input_handler_.get())
      SetupInputRouter();

    mojom::WidgetInputHandlerHostPtr host;
    mojom::WidgetInputHandlerHostRequest host_request =
        mojo::MakeRequest(&host);
    widget->SetupWidgetInputHandler(mojo::MakeRequest(&widget_input_handler_),
                                    std::move(host));
    input_router_->BindHost(std::move(host_request), false);
  }
}
