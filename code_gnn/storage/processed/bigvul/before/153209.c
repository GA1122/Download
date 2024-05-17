void DesktopWindowTreeHostX11::Deactivate() {
  BeforeActivationStateChanged();

  ignore_keyboard_input_ = true;

  ReleaseCapture();
  XLowerWindow(xdisplay_, xwindow_);

  AfterActivationStateChanged();
}
