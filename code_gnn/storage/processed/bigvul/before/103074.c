 bool Browser::CanBookmarkAllTabs() const {
  BookmarkModel* model = profile()->GetBookmarkModel();
  return (model && model->IsLoaded()) &&
         tab_count() > 1 &&
         edit_bookmarks_enabled_.GetValue();
}
