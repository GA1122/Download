void DesktopWindowTreeHostX11::SetUseNativeFrame(bool use_native_frame) {
  use_native_frame_ = use_native_frame;
  ui::SetUseOSWindowFrame(xwindow_, use_native_frame);
  ResetWindowRegion();
}
