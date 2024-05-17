void DesktopWindowTreeHostX11::DispatchTouchEvent(ui::TouchEvent* event) {
  if (g_current_capture && g_current_capture != this &&
      event->type() == ui::ET_TOUCH_PRESSED) {
    DCHECK_EQ(ui::GetScaleFactorForNativeView(window()),
              ui::GetScaleFactorForNativeView(g_current_capture->window()));
    ConvertEventLocationToTargetWindowLocation(
        g_current_capture->GetLocationOnScreenInPixels(),
        GetLocationOnScreenInPixels(), event->AsLocatedEvent());
    g_current_capture->SendEventToSink(event);
  } else {
    SendEventToSink(event);
  }
}
