BOOL CALLBACK ShowWindowsCallback(HWND window, LPARAM param) {
  RenderWidgetHostViewAura* widget =
      reinterpret_cast<RenderWidgetHostViewAura*>(param);

  if (GetProp(window, kWidgetOwnerProperty) == widget &&
      widget->GetNativeView()->GetDispatcher()) {
    HWND parent = widget->GetNativeView()->GetDispatcher()->host()->
        GetAcceleratedWidget();
    SetParent(window, parent);
  }
  return TRUE;
}
