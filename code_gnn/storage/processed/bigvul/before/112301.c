ShellWindow::~ShellWindow() {
  registrar_.RemoveAll();

  browser::EndKeepAlive();
}
