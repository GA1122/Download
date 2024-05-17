void ShelfLayoutManager::UpdateShelfBackground(
    BackgroundAnimator::ChangeType type) {
  bool launcher_paints = GetLauncherPaintsBackground();
  if (launcher_)
    launcher_->SetPaintsBackground(launcher_paints, type);
  StatusAreaWidget* status_area_widget =
      Shell::GetInstance()->status_area_widget();
  if (status_area_widget)
    status_area_widget->SetPaintsBackground(!launcher_paints, type);
}
