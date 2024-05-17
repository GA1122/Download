void HWNDMessageHandler::FrameTypeChanged() {

  UpdateDwmNcRenderingPolicy();

  ResetWindowRegion(true, false);

  delegate_->HandleFrameChanged();

  if (IsVisible() && !delegate_->IsUsingCustomFrame()) {
    UINT flags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER;
    SetWindowPos(hwnd(), NULL, 0, 0, 0, 0, flags | SWP_HIDEWINDOW);
    SetWindowPos(hwnd(), NULL, 0, 0, 0, 0, flags | SWP_SHOWWINDOW);

    UpdateWindow(hwnd());
  }

  EnumChildWindows(hwnd(), &SendDwmCompositionChanged, NULL);
}
