void RenderWidgetHostImpl::OnWindowlessPluginDummyWindowCreated(
    gfx::NativeViewId dummy_activation_window) {
  HWND hwnd = reinterpret_cast<HWND>(dummy_activation_window);

  wchar_t window_title[MAX_PATH + 1] = {0};
  if (!IsWindow(hwnd) ||
      !GetWindowText(hwnd, window_title, arraysize(window_title)) ||
      lstrcmpiW(window_title, kDummyActivationWindowName) != 0) {
    return;
  }

#if defined(USE_AURA)
  SetParent(hwnd,
            reinterpret_cast<HWND>(view_->GetParentForWindowlessPlugin()));
#else
  SetParent(hwnd, reinterpret_cast<HWND>(GetNativeViewId()));
#endif
  dummy_windows_for_activation_.push_back(hwnd);
}
