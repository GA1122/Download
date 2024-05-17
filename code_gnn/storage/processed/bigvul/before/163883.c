DevToolsToolboxDelegate::PreHandleKeyboardEvent(
    content::WebContents* source,
    const content::NativeWebKeyboardEvent& event) {
  BrowserWindow* window = GetInspectedBrowserWindow();
  if (window)
    return window->PreHandleKeyboardEvent(event);
  return content::KeyboardEventProcessingResult::NOT_HANDLED;
}
