void DesktopWindowTreeHostX11::OnNativeWidgetCreated(
    const Widget::InitParams& params) {
  window()->SetProperty(kViewsWindowForRootWindow, content_window());
  window()->SetProperty(kHostForRootWindow, this);

  X11DesktopHandler::get();

  SwapNonClientEventHandler(
      std::unique_ptr<ui::EventHandler>(new X11WindowEventFilter(this)));
  SetUseNativeFrame(params.type == Widget::InitParams::TYPE_WINDOW &&
                    !params.remove_standard_frame);

  x11_window_move_client_ = std::make_unique<X11DesktopWindowMoveClient>();
  wm::SetWindowMoveClient(window(), x11_window_move_client_.get());

  SetWindowTransparency();

  native_widget_delegate_->OnNativeWidgetCreated();
}
