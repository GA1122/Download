BOOL CALLBACK ShowWindowsCallback(HWND window, LPARAM param) {
  RenderWidgetHostViewAura* widget =
      reinterpret_cast<RenderWidgetHostViewAura*>(param);

  HWND parent =
      widget->GetNativeView()->GetRootWindow()->GetAcceleratedWidget();
  if (GetProp(window, kWidgetOwnerProperty) == widget)
    SetParent(window, parent);
  return TRUE;
}
