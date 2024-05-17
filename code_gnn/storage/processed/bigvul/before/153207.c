NonClientFrameView* DesktopWindowTreeHostX11::CreateNonClientFrameView() {
  return ShouldUseNativeFrame()
             ? new NativeFrameView(native_widget_delegate_->AsWidget())
             : nullptr;
}
