void WebPluginProxy::DidPaint() {
  waiting_for_paint_ = false;
  if (!damaged_rect_.IsEmpty())
     InvalidateRect(damaged_rect_);
 }
