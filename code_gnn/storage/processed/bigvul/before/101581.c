bool Browser::CanBookmarkAllTabs() const {
  BookmarkModel* model = profile()->GetBookmarkModel();
  return (model && model->IsLoaded()) &&
         tab_count() > 1 &&
         profile()->GetPrefs()->GetBoolean(prefs::kEditBookmarksEnabled);
}
