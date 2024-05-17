void RenderWidgetHostImpl::OnWindowlessPluginDummyWindowDestroyed(
    gfx::NativeViewId dummy_activation_window) {
  HWND hwnd = reinterpret_cast<HWND>(dummy_activation_window);
  std::list<HWND>::iterator i = dummy_windows_for_activation_.begin();
  for (; i != dummy_windows_for_activation_.end(); ++i) {
    if ((*i) == hwnd) {
      dummy_windows_for_activation_.erase(i);
      return;
    }
  }
  NOTREACHED() << "Unknown dummy window";
}
