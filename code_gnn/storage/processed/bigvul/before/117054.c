Browser::Type SessionService::BrowserTypeForWindowType(WindowType type) {
  switch (type) {
    case TYPE_POPUP:
      return Browser::TYPE_POPUP;
    case TYPE_TABBED:
    default:
      return Browser::TYPE_TABBED;
  }
}
