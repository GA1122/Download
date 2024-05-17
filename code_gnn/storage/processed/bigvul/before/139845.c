void Shell::OnRootWindowAdded(aura::Window* root_window) {
  FOR_EACH_OBSERVER(ShellObserver, observers_, OnRootWindowAdded(root_window));
}
