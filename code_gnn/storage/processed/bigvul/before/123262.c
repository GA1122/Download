BOOL CALLBACK WindowDestroyingCallback(HWND window, LPARAM param) {
  RenderWidgetHostViewAura* widget =
      reinterpret_cast<RenderWidgetHostViewAura*>(param);
  if (GetProp(window, kWidgetOwnerProperty) == widget) {
    RemoveProp(window, kWidgetOwnerProperty);
    RenderWidgetHostViewBase::DetachPluginWindowsCallback(window);
  }
  return TRUE;
}
