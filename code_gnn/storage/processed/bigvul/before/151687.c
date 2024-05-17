void Browser::HandleKeyboardEvent(content::WebContents* source,
                                  const NativeWebKeyboardEvent& event) {
  DevToolsWindow* devtools_window =
      DevToolsWindow::GetInstanceForInspectedWebContents(source);
  bool handled = false;
  if (devtools_window)
    handled = devtools_window->ForwardKeyboardEvent(event);

  if (!handled)
    window()->HandleKeyboardEvent(event);
}
