void RenderWidgetHostImpl::OnWindowlessPluginDummyWindowCreated(
    gfx::NativeViewId dummy_activation_window) {
  HWND hwnd = reinterpret_cast<HWND>(dummy_activation_window);
  if (!IsWindow(hwnd) ||
      !WebPluginDelegateImpl::IsDummyActivationWindow(hwnd)) {
    return;
  }

  SetParent(hwnd, reinterpret_cast<HWND>(GetNativeViewId()));
  dummy_windows_for_activation_.push_back(hwnd);
}
