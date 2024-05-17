BOOL CALLBACK FindOwnedWindowsCallback(HWND hwnd, LPARAM param) {
#if !defined(USE_AURA)
  FindOwnedWindowsData* data = reinterpret_cast<FindOwnedWindowsData*>(param);
  if (GetWindow(hwnd, GW_OWNER) == data->window) {
    Widget* widget = Widget::GetWidgetForNativeView(hwnd);
    if (widget)
      data->owned_widgets.push_back(widget);
  }
#endif
  return TRUE;
}
