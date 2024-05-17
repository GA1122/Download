void RenderViewImpl::OnThemeChanged() {
#if defined(USE_AURA)
#elif defined(OS_WIN)
  gfx::NativeThemeWin::instance()->CloseHandles();
  if (webview())
    webview()->themeChanged();
#else   
  NOTIMPLEMENTED();
#endif
}
