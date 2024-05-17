DevToolsWindow::~DevToolsWindow() {
  life_stage_ = kClosing;

  UpdateBrowserWindow();
  UpdateBrowserToolbar();

  if (toolbox_web_contents_)
    delete toolbox_web_contents_;

  DevToolsWindows* instances = g_instances.Pointer();
  DevToolsWindows::iterator it(
      std::find(instances->begin(), instances->end(), this));
  DCHECK(it != instances->end());
  instances->erase(it);

  if (!close_callback_.is_null()) {
    close_callback_.Run();
    close_callback_ = base::Closure();
  }
}
