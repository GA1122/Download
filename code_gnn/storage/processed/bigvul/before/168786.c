void RenderWidgetHostImpl::SetupInputRouter() {
  in_flight_event_count_ = 0;
  StopHangMonitorTimeout();
  associated_widget_input_handler_ = nullptr;
  widget_input_handler_ = nullptr;

  if (base::FeatureList::IsEnabled(features::kMojoInputMessages)) {
    input_router_.reset(
        new InputRouterImpl(this, this, GetInputRouterConfigForPlatform()));
    legacy_widget_input_handler_ =
        std::make_unique<UnboundWidgetInputHandler>();
  } else {
    input_router_.reset(new LegacyInputRouterImpl(
        process_, this, this, routing_id_, GetInputRouterConfigForPlatform()));
    legacy_widget_input_handler_ =
        std::make_unique<LegacyIPCWidgetInputHandler>(
            static_cast<LegacyInputRouterImpl*>(input_router_.get()));
  }

  if (IsUseZoomForDSFEnabled()) {
    input_router_->SetDeviceScaleFactor(GetScaleFactorForView(view_.get()));
  }
}
