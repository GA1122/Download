void ExtensionGlobalError::ShowBubbleView(Browser* browser) {
  current_browser_ = browser;
  GlobalError::ShowBubbleView(browser);
}
