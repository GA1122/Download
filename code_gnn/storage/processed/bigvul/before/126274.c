void BrowserCommandController::FullscreenStateChanged() {
  FullScreenMode fullscreen_mode = FULLSCREEN_DISABLED;
  if (window()->IsFullscreen()) {
#if defined(OS_WIN)
    fullscreen_mode = window()->IsInMetroSnapMode() ? FULLSCREEN_METRO_SNAP :
                                                      FULLSCREEN_NORMAL;
#else
    fullscreen_mode = FULLSCREEN_NORMAL;
#endif
  }
  UpdateCommandsForFullscreenMode(fullscreen_mode);
}
