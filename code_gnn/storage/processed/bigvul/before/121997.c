void AppListController::ShowAppListDuringModeSwitch(
    Profile* requested_profile) {
  regain_first_lost_focus_ = true;
  ShowAppList(requested_profile);
}
