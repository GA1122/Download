void Browser::OnWindowActivated() {
  TabContents* contents = GetSelectedTabContents();
  if (contents && contents->crashed_status() ==
     base::TERMINATION_STATUS_PROCESS_WAS_KILLED) {
    if (CommandLine::ForCurrentProcess()->HasSwitch(
            switches::kReloadKilledTabs)) {
      Reload(CURRENT_TAB);
    }
  }
}
