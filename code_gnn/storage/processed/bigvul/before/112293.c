void ShellWindow::OnNativeClose() {
  ShellWindowRegistry::Get(profile_)->RemoveShellWindow(this);
  delete this;
}
