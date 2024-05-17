HWND AppListController::GetAppListHWND() const {
#if defined(USE_AURA)
  gfx::NativeWindow window =
      current_view_->GetWidget()->GetTopLevelWidget()->GetNativeWindow();
  return window->GetRootWindow()->GetAcceleratedWidget();
#else
  return current_view_->GetWidget()->GetTopLevelWidget()->GetNativeWindow();
#endif
}
