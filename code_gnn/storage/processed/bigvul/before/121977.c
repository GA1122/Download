gfx::NativeWindow AppListController::GetAppListWindow() {
  if (!IsAppListVisible())
    return NULL;
  return current_view_ ? current_view_->GetWidget()->GetNativeWindow() : NULL;
}
