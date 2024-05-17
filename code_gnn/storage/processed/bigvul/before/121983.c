bool AppListController::IsAppListVisible() const {
  return current_view_ && current_view_->GetWidget()->IsVisible();
}
