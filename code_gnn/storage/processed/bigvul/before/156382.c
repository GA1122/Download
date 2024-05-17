void PageHandler::CaptureScreenshot(
    Maybe<std::string> format,
    Maybe<int> quality,
    Maybe<Page::Viewport> clip,
    Maybe<bool> from_surface,
    std::unique_ptr<CaptureScreenshotCallback> callback) {
  if (!host_ || !host_->GetRenderWidgetHost() ||
      !host_->GetRenderWidgetHost()->GetView()) {
    callback->sendFailure(Response::InternalError());
    return;
  }

  RenderWidgetHostImpl* widget_host = host_->GetRenderWidgetHost();
  std::string screenshot_format = format.fromMaybe(kPng);
  int screenshot_quality = quality.fromMaybe(kDefaultScreenshotQuality);

  if (!from_surface.fromMaybe(true)) {
    widget_host->GetSnapshotFromBrowser(
        base::Bind(&PageHandler::ScreenshotCaptured, weak_factory_.GetWeakPtr(),
                   base::Passed(std::move(callback)), screenshot_format,
                   screenshot_quality, gfx::Size(), gfx::Size(),
                   blink::WebDeviceEmulationParams()),
        false);
    return;
  }

  bool emulation_enabled = emulation_handler_->device_emulation_enabled();
  blink::WebDeviceEmulationParams original_params =
      emulation_handler_->GetDeviceEmulationParams();
  blink::WebDeviceEmulationParams modified_params = original_params;

  gfx::Size original_view_size =
      emulation_enabled || clip.isJust()
          ? widget_host->GetView()->GetViewBounds().size()
          : gfx::Size();
  gfx::Size emulated_view_size = modified_params.view_size;

  double dpfactor = 1;
  ScreenInfo screen_info;
  widget_host->GetScreenInfo(&screen_info);
  if (emulation_enabled) {
    float original_scale =
        original_params.scale > 0 ? original_params.scale : 1;
    if (!modified_params.view_size.width) {
      emulated_view_size.set_width(
          ceil(original_view_size.width() / original_scale));
    }
    if (!modified_params.view_size.height) {
      emulated_view_size.set_height(
          ceil(original_view_size.height() / original_scale));
    }

    dpfactor = modified_params.device_scale_factor
                   ? modified_params.device_scale_factor /
                         screen_info.device_scale_factor
                   : 1;
    modified_params.scale = clip.isJust() ? 1 : dpfactor;
    modified_params.view_size.width = emulated_view_size.width();
    modified_params.view_size.height = emulated_view_size.height();
  } else if (clip.isJust()) {
    modified_params.view_size.width = original_view_size.width();
    modified_params.view_size.height = original_view_size.height();
    modified_params.screen_size.width = 0;
    modified_params.screen_size.height = 0;
    modified_params.device_scale_factor = 0;
    modified_params.scale = 1;
  }

  if (clip.isJust()) {
    modified_params.viewport_offset.x = clip.fromJust()->GetX();
    modified_params.viewport_offset.y = clip.fromJust()->GetY();
    modified_params.viewport_scale = clip.fromJust()->GetScale() * dpfactor;
  }

  emulation_handler_->SetDeviceEmulationParams(modified_params);

  if (clip.isJust()) {
    double scale = dpfactor * clip.fromJust()->GetScale();
    widget_host->GetView()->SetSize(
        gfx::Size(gfx::ToRoundedInt(clip.fromJust()->GetWidth() * scale),
                  gfx::ToRoundedInt(clip.fromJust()->GetHeight() * scale)));
  } else if (emulation_enabled) {
    widget_host->GetView()->SetSize(
        gfx::ScaleToFlooredSize(emulated_view_size, dpfactor));
  }
  gfx::Size requested_image_size = gfx::Size();
  if (emulation_enabled || clip.isJust()) {
    if (clip.isJust()) {
      requested_image_size =
          gfx::Size(clip.fromJust()->GetWidth(), clip.fromJust()->GetHeight());
    } else {
      requested_image_size = emulated_view_size;
    }
    double scale = emulation_enabled ? original_params.device_scale_factor
                                     : screen_info.device_scale_factor;
    if (clip.isJust())
      scale *= clip.fromJust()->GetScale();
    requested_image_size = gfx::ScaleToRoundedSize(requested_image_size, scale);
  }

  widget_host->GetSnapshotFromBrowser(
      base::Bind(&PageHandler::ScreenshotCaptured, weak_factory_.GetWeakPtr(),
                 base::Passed(std::move(callback)), screenshot_format,
                 screenshot_quality, original_view_size, requested_image_size,
                 original_params),
      true);
}
