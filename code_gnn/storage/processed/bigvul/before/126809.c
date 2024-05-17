bool BrowserView::IsInMetroSnapMode() const {
#if defined(USE_AURA)
  return false;
#else
  return static_cast<views::NativeWidgetWin*>(
      frame_->native_widget())->IsInMetroSnapMode();
#endif
}
