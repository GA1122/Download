bool DesktopWindowTreeHostX11::CanDispatchEvent(
    const ui::PlatformEvent& event) {
  return event->xany.window == xwindow_ ||
         (event->type == GenericEvent &&
          static_cast<XIDeviceEvent*>(event->xcookie.data)->event == xwindow_);
}
