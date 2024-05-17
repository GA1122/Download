bool RenderViewImpl::DidTapMultipleTargets(
    const WebSize& inner_viewport_offset,
    const WebRect& touch_rect,
    const WebVector<WebRect>& target_rects) {
  ui::AXMode accessibility_mode = GetMainRenderFrame()->accessibility_mode();
  if (accessibility_mode == ui::kAXModeComplete)
    return false;

  gfx::Rect zoom_rect;
  float new_total_scale =
      DisambiguationPopupHelper::ComputeZoomAreaAndScaleFactor(
          touch_rect, target_rects, GetSize(),
          gfx::Rect(webview()->MainFrame()->VisibleContentRect()).size(),
          device_scale_factor_ * webview()->PageScaleFactor(), &zoom_rect);
  if (!new_total_scale || zoom_rect.IsEmpty())
    return false;

  bool handled = false;
  switch (renderer_preferences_.tap_multiple_targets_strategy) {
    case TAP_MULTIPLE_TARGETS_STRATEGY_ZOOM:
      handled = webview()->ZoomToMultipleTargetsRect(zoom_rect);
      break;
    case TAP_MULTIPLE_TARGETS_STRATEGY_POPUP: {
      gfx::Size canvas_size =
          gfx::ScaleToCeiledSize(zoom_rect.size(), new_total_scale);

      SkImageInfo info =
          SkImageInfo::MakeN32Premul(canvas_size.width(), canvas_size.height());
      size_t shm_size = info.computeMinByteSize();

      if (shm_size == 0) {
        DLOG(ERROR) << "Invalid size for SharedMemory";
        return false;
      }

      auto shm = ChildThreadImpl::AllocateSharedMemory(shm_size);
      if (!shm || !shm->Map(shm_size)) {
        DLOG(ERROR) << "SharedMemory allocate/map failed";
        return false;
      }

      {
        SkBitmap bitmap;
        bitmap.installPixels(info, shm->memory(), info.minRowBytes());
        cc::SkiaPaintCanvas canvas(bitmap);

        canvas.scale(new_total_scale / device_scale_factor_,
                     new_total_scale / device_scale_factor_);
        canvas.translate(-zoom_rect.x() * device_scale_factor_,
                         -zoom_rect.y() * device_scale_factor_);

        DCHECK(webview_->IsAcceleratedCompositingActive());
        webview_->UpdateAllLifecyclePhases();
        webview_->PaintIgnoringCompositing(&canvas, zoom_rect);
      }

      gfx::Rect zoom_rect_in_screen =
          zoom_rect - gfx::Vector2d(inner_viewport_offset.width,
                                    inner_viewport_offset.height);

      gfx::Rect physical_window_zoom_rect = gfx::ToEnclosingRect(
          ClientRectToPhysicalWindowRect(gfx::RectF(zoom_rect_in_screen)));

      Send(new ViewHostMsg_ShowDisambiguationPopup(
          GetRoutingID(), physical_window_zoom_rect, canvas_size,
          shm->TakeHandle()));

      handled = true;
      break;
    }
    case TAP_MULTIPLE_TARGETS_STRATEGY_NONE:
      break;
  }

  return handled;
}
