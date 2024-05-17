void DesktopWindowTreeHostX11::DispatchKeyEvent(ui::KeyEvent* event) {
  if (native_widget_delegate_->AsWidget()->IsActive())
    SendEventToSink(event);
}
