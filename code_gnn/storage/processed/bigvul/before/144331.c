void LoginDisplayHostWebUI::ScheduleWorkspaceAnimation() {
  if (!features::IsAshInBrowserProcess()) {
    NOTIMPLEMENTED();
    return;
  }

  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableLoginAnimations)) {
    ash::Shell::Get()->DoInitialWorkspaceAnimation();
  }
}
