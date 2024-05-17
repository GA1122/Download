bool Browser::ShouldOpenNewTabForWindowDisposition(
    WindowOpenDisposition disposition) {
  return (disposition == NEW_FOREGROUND_TAB ||
          disposition == NEW_BACKGROUND_TAB);
}
