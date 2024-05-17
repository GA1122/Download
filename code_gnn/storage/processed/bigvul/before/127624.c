void SetHideTitlebarWhenMaximizedProperty(XID window,
                                          HideTitlebarWhenMaximized property) {
  uint32 hide = property;
  XChangeProperty(GetXDisplay(),
      window,
      GetAtom("_GTK_HIDE_TITLEBAR_WHEN_MAXIMIZED"),
      XA_CARDINAL,
      32,   
      PropModeReplace,
      reinterpret_cast<unsigned char*>(&hide),
      1);
}
