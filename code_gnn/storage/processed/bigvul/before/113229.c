void ShelfLayoutManager::LayoutShelf() {
  AutoReset<bool> auto_reset_in_layout(&in_layout_, true);
  StopAnimating();
  TargetBounds target_bounds;
  CalculateTargetBounds(state_, &target_bounds);
  if (launcher_widget()) {
    GetLayer(launcher_widget())->SetOpacity(target_bounds.opacity);

    launcher_widget()->SetBounds(target_bounds.launcher_bounds);
    launcher_->SetStatusSize(target_bounds.status_bounds.size());
  }
  GetLayer(status_)->SetOpacity(target_bounds.opacity);
  status_->SetBounds(target_bounds.status_bounds);
  Shell::GetInstance()->SetDisplayWorkAreaInsets(
      Shell::GetPrimaryRootWindow(),
      target_bounds.work_area_insets);
  UpdateHitTestBounds();
}
