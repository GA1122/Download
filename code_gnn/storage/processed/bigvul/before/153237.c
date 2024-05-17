std::string DesktopWindowTreeHostX11::GetWorkspace() const {
  return workspace_ ? base::NumberToString(workspace_.value()) : std::string();
}
