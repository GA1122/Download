void Shell::NotifyFullscreenStateChange(bool is_fullscreen,
                                        aura::Window* root_window) {
  FOR_EACH_OBSERVER(ShellObserver, observers_, OnFullscreenStateChanged(
      is_fullscreen, root_window));
}
