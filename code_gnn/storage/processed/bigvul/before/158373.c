void RenderWidgetHostImpl::SetupInputRouter() {
  in_flight_event_count_ = 0;
  StopInputEventAckTimeout();
  associated_widget_input_handler_ = nullptr;
  widget_input_handler_ = nullptr;

  input_router_.reset(new InputRouterImpl(this, this, fling_scheduler_.get(),
                                          GetInputRouterConfigForPlatform()));

  input_router_->SetForceEnableZoom(force_enable_zoom_);

  if (IsUseZoomForDSFEnabled()) {
    input_router_->SetDeviceScaleFactor(GetScaleFactorForView(view_.get()));
  }
}
