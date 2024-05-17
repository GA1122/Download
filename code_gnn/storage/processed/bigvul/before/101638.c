void Browser::ExitTabbedFullscreenModeIfNecessary() {
  if (tab_caused_fullscreen_)
    ToggleFullscreenMode();
  else
    NotifyTabOfFullscreenExitIfNecessary();
}
