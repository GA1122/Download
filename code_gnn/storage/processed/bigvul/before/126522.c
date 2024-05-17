GtkWidget* TabStripGtk::GetWidgetForViewID(ViewID view_id) {
  if (GetTabCount() > 0) {
    if (view_id == VIEW_ID_TAB_LAST) {
      return GetTabAt(GetTabCount() - 1)->widget();
    } else if ((view_id >= VIEW_ID_TAB_0) && (view_id < VIEW_ID_TAB_LAST)) {
      int index = view_id - VIEW_ID_TAB_0;
      if (index >= 0 && index < GetTabCount()) {
        return GetTabAt(index)->widget();
      } else {
        return NULL;
      }
    }
  }

  return NULL;
}
