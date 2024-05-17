bool DesktopWindowTreeHostX11::IsVisibleOnAllWorkspaces() const {
  return GetWorkspace() == base::NumberToString(kAllDesktops);
}
