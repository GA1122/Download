void InfoBar::AnimateClose() {
  bool restore_focus = true;
#if defined(OS_WIN)
  if (GetWidget() &&
      !ui::DoesWindowBelongToActiveWindow(GetWidget()->GetNativeView())) {
    restore_focus = false;
  }
#endif   
  DestroyFocusTracker(restore_focus);
  animation_->Hide();
}
