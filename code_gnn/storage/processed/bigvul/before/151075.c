void DevToolsToolboxDelegate::HandleKeyboardEvent(
    content::WebContents* source,
    const content::NativeWebKeyboardEvent& event) {
  if (event.windowsKeyCode == 0x08) {
    return;
  }
  BrowserWindow* window = GetInspectedBrowserWindow();
  if (window)
    window->HandleKeyboardEvent(event);
}
