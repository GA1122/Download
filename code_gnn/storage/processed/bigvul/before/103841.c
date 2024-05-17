void RenderView::OnThemeChanged() {
#if defined(OS_WIN)
  gfx::NativeThemeWin::instance()->CloseHandles();
  if (webview())
    webview()->themeChanged();
#else   
  NOTIMPLEMENTED();
#endif
}
