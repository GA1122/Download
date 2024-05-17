void Shell::OnOverviewModeStarting() {
  FOR_EACH_OBSERVER(ShellObserver, observers_, OnOverviewModeStarting());
}
