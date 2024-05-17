gfx::NativeWindow BrowserView::GetNativeWindow() const {
  return GetWidget() ? GetWidget()->GetNativeWindow() : nullptr;
}
