SessionService::WindowType SessionService::WindowTypeForBrowserType(
    Browser::Type type) {
  switch (type) {
    case Browser::TYPE_POPUP:
      return TYPE_POPUP;
    case Browser::TYPE_TABBED:
      return TYPE_TABBED;
    default:
      DCHECK(false);
      return TYPE_TABBED;
  }
}
