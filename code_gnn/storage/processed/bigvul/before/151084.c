void DevToolsWindow::LoadCompleted() {
  Show(action_on_load_);
  action_on_load_ = DevToolsToggleAction::NoOp();
  if (!load_completed_callback_.is_null()) {
    load_completed_callback_.Run();
    load_completed_callback_ = base::Closure();
  }
}
