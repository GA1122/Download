void PageHandler::ScreenshotCaptured(
    std::unique_ptr<CaptureScreenshotCallback> callback,
    const std::string& format,
    int quality,
    const gfx::Size& original_view_size,
    const gfx::Size& requested_image_size,
    const blink::WebDeviceEmulationParams& original_emulation_params,
    const gfx::Image& image) {
  if (original_view_size.width()) {
    RenderWidgetHostImpl* widget_host = host_->GetRenderWidgetHost();
    widget_host->GetView()->SetSize(original_view_size);
    emulation_handler_->SetDeviceEmulationParams(original_emulation_params);
  }

  if (image.IsEmpty()) {
    callback->sendFailure(Response::Error("Unable to capture screenshot"));
    return;
  }

  if (!requested_image_size.IsEmpty() &&
      (image.Width() != requested_image_size.width() ||
       image.Height() != requested_image_size.height())) {
    const SkBitmap* bitmap = image.ToSkBitmap();
    SkBitmap cropped = SkBitmapOperations::CreateTiledBitmap(
        *bitmap, 0, 0, requested_image_size.width(),
        requested_image_size.height());
    gfx::Image croppedImage = gfx::Image::CreateFrom1xBitmap(cropped);
    callback->sendSuccess(EncodeImage(croppedImage, format, quality));
  } else {
    callback->sendSuccess(EncodeImage(image, format, quality));
  }
}
