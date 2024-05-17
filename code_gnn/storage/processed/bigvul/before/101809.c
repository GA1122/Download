bool Browser::ShouldSaveWindowPlacement() const {
  switch (type_) {
    case TYPE_TABBED:
      return true;
    case TYPE_POPUP:
      return browser_defaults::kRestorePopups || is_devtools();
    case TYPE_PANEL:
      return false;
    default:
      return false;
  }
}
