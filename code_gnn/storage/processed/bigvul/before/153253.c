bool DesktopWindowTreeHostX11::IsTranslucentWindowOpacitySupported() const {
  return ui::XVisualManager::GetInstance()->ArgbVisualAvailable();
}
