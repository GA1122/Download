bool DevToolsWindow::IsDevToolsWindow(content::WebContents* web_contents) {
  if (!web_contents || g_instances == NULL)
    return false;
  DevToolsWindows* instances = g_instances.Pointer();
  for (DevToolsWindows::iterator it(instances->begin()); it != instances->end();
       ++it) {
    if ((*it)->main_web_contents_ == web_contents ||
        (*it)->toolbox_web_contents_ == web_contents)
      return true;
  }
  return false;
}
