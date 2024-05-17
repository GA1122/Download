gfx::NativeWindow BrowserView::GetNativeWindow() {
  return GetWidget()->GetTopLevelWidget()->GetNativeWindow();
}
