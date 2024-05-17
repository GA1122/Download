void PanelBrowserView::EnsurePanelFullyVisible() {
#if defined(OS_WIN) && !defined(USE_AURA)
  ::SetWindowPos(GetNativeHandle(), HWND_TOP, 0, 0, 0, 0,
                 SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
#else
  NOTIMPLEMENTED();
#endif
}
