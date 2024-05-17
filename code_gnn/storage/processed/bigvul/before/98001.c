void RenderView::OnThemeChanged() {
#if defined(OS_WIN)
  gfx::NativeTheme::instance()->CloseHandles();
  didInvalidateRect(gfx::Rect(size_));
#else   
  NOTIMPLEMENTED();
#endif
}