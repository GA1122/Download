void RenderWidgetHostImpl::GetScreenInfo(ScreenInfo* result) {
  TRACE_EVENT0("renderer_host", "RenderWidgetHostImpl::GetScreenInfo");
  if (view_)
    view_->GetScreenInfo(result);
  else
    DisplayUtil::GetDefaultScreenInfo(result);

  if (IsUseZoomForDSFEnabled())
    input_router_->SetDeviceScaleFactor(result->device_scale_factor);
}
