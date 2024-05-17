void Shell::OnOverviewModeEnded() {
  FOR_EACH_OBSERVER(ShellObserver, observers_, OnOverviewModeEnded());
}
