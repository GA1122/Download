bool BrowserWindowGtk::DrawFrameAsActive() const {
  if (ui::ActiveWindowWatcherX::WMSupportsActivation())
    return is_active_;

  return true;
}
