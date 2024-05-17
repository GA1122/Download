void AppListController::OnLoadProfileForWarmup(Profile* initial_profile) {
  if (!IsWarmupNeeded())
    return;

  PopulateViewFromProfile(initial_profile);
  current_view_->Prerender();
}
