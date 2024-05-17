bool RenderViewImpl::didTapMultipleTargets(
    const WebKit::WebGestureEvent& event,
    const WebVector<WebRect>& target_rects) {
  gfx::Rect finger_rect(
      event.x - event.data.tap.width / 2, event.y - event.data.tap.height / 2,
      event.data.tap.width, event.data.tap.height);
  gfx::Rect zoom_rect;
  float scale = DisambiguationPopupHelper::ComputeZoomAreaAndScaleFactor(
      finger_rect, target_rects, GetSize(), &zoom_rect);
  if (!scale)
    return false;

  gfx::Size canvas_size = zoom_rect.size();
  canvas_size = ToCeiledSize(gfx::ScaleSize(canvas_size, scale));
  TransportDIB* transport_dib = NULL;
  {
    scoped_ptr<skia::PlatformCanvas> canvas(
        RenderProcess::current()->GetDrawingCanvas(&transport_dib,
                                                   gfx::Rect(canvas_size)));
    if (!canvas.get())
      return false;

    canvas->scale(scale, scale);

    canvas->translate(-zoom_rect.x(), -zoom_rect.y());
    webwidget_->paint(webkit_glue::ToWebCanvas(canvas.get()), zoom_rect,
        WebWidget::ForceSoftwareRenderingAndIgnoreGPUResidentContent);
  }
  Send(new ViewHostMsg_ShowDisambiguationPopup(routing_id_,
                                               zoom_rect,
                                               canvas_size,
                                               transport_dib->id()));

  return true;
}
