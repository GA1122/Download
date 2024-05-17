void DesktopWindowTreeHostX11::UpdateWorkspace() {
  int workspace;
  if (ui::GetWindowDesktop(xwindow_, &workspace))
    workspace_ = workspace;
  else
    workspace_ = base::nullopt;
}
