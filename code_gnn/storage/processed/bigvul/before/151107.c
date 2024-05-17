void DevToolsWindow::ScheduleShow(const DevToolsToggleAction& action) {
  if (life_stage_ == kLoadCompleted) {
    Show(action);
    return;
  }

  action_on_load_ = action;

  if (!can_dock_) {
    is_docked_ = false;
    Show(DevToolsToggleAction::Show());
  }
}
