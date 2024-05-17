void Shell::OnMaximizeModeEnded() {
  FOR_EACH_OBSERVER(ShellObserver, observers_, OnMaximizeModeEnded());
}
