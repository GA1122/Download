bool RenderViewImpl::didTapMultipleTargets(
    const WebKit::WebGestureEvent& event,
    const WebVector<WebRect>& target_rects) {
  gfx::Rect finger_rect(
      event.x - event.data.tap.width / 2, event.y - event.data.tap.height / 2,
      event.data.tap.width, event.data.tap.height);
  gfx::Rect zoom_rect;
  float new_total_scale =
      DisambiguationPopupHelper::ComputeZoomAreaAndScaleFactor(
          finger_rect, target_rects, GetSize(),
          gfx::Rect(webview()->mainFrame()->visibleContentRect()).size(),
          device_scale_factor_ * webview()->pageScaleFactor(), &zoom_rect);
  if (!new_total_scale)
    return false;

  gfx::Size canvas_size = gfx::ToCeiledSize(gfx::ScaleSize(zoom_rect.size(),
                                                           new_total_scale));
  TransportDIB* transport_dib = NULL;
  {
    scoped_ptr<skia::PlatformCanvas> canvas(
        RenderProcess::current()->GetDrawingCanvas(&transport_dib,
                                                   gfx::Rect(canvas_size)));
    if (!canvas.get())
      return false;

    canvas->scale(new_total_scale / device_scale_factor_,
                  new_total_scale / device_scale_factor_);
    canvas->translate(-zoom_rect.x() * device_scale_factor_,
                      -zoom_rect.y() * device_scale_factor_);

    webwidget_->paint(webkit_glue::ToWebCanvas(canvas.get()), zoom_rect,
        WebWidget::ForceSoftwareRenderingAndIgnoreGPUResidentContent);
  }

  gfx::Rect physical_window_zoom_rect = gfx::ToEnclosingRect(
      ClientRectToPhysicalWindowRect(gfx::RectF(zoom_rect)));
  Send(new ViewHostMsg_ShowDisambiguationPopup(routing_id_,
                                               physical_window_zoom_rect,
                                               canvas_size,
                                               transport_dib->id()));

  return true;
}
