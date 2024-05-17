bool LauncherView::SameDragType(LauncherItemType typea,
                                LauncherItemType typeb) const {
  switch (typea) {
    case TYPE_TABBED:
    case TYPE_APP_PANEL:
    case TYPE_PLATFORM_APP:
      return (typeb == TYPE_TABBED ||
          typeb == TYPE_APP_PANEL ||
          typeb == TYPE_PLATFORM_APP);
    case TYPE_APP_SHORTCUT:
    case TYPE_APP_LIST:
    case TYPE_BROWSER_SHORTCUT:
      return typeb == typea;
  }
  NOTREACHED();
  return false;
}
