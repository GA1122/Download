void ExtensionHelper::AddErrorToRootConsole(const string16& message) {
  if (render_view()->GetWebView() && render_view()->GetWebView()->mainFrame()) {
    render_view()->GetWebView()->mainFrame()->addMessageToConsole(
        WebConsoleMessage(WebConsoleMessage::LevelError, message));
  }
}
