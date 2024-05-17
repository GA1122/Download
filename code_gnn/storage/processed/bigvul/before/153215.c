void DesktopWindowTreeHostX11::DispatchMouseEvent(ui::MouseEvent* event) {
  if (content_window() && content_window()->delegate()) {
    int flags = event->flags();
    gfx::Point location_in_dip = event->location();
    GetRootTransform().TransformPointReverse(&location_in_dip);
    int hit_test_code =
        content_window()->delegate()->GetNonClientComponent(location_in_dip);
    if (hit_test_code != HTCLIENT && hit_test_code != HTNOWHERE)
      flags |= ui::EF_IS_NON_CLIENT;
    event->set_flags(flags);
  }

  if (event->IsAnyButton() || event->IsMouseWheelEvent())
    FlashFrame(false);

  if (!g_current_capture || g_current_capture == this) {
    SendEventToSink(event);
  } else {
    DCHECK_EQ(ui::GetScaleFactorForNativeView(window()),
              ui::GetScaleFactorForNativeView(g_current_capture->window()));
    ConvertEventLocationToTargetWindowLocation(
        g_current_capture->GetLocationOnScreenInPixels(),
        GetLocationOnScreenInPixels(), event->AsLocatedEvent());
    g_current_capture->SendEventToSink(event);
  }
}
