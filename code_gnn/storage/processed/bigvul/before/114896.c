void TestingAutomationProvider::GetBookmarksAsJSON(
    int handle,
    std::string* bookmarks_as_json,
    bool *success) {
  *success = false;
  if (browser_tracker_->ContainsHandle(handle)) {
    Browser* browser = browser_tracker_->GetResource(handle);
    if (browser) {
      if (!browser->profile()->GetBookmarkModel()->IsLoaded()) {
        return;
      }
      scoped_refptr<BookmarkStorage> storage(new BookmarkStorage(
          browser->profile(),
          browser->profile()->GetBookmarkModel()));
      *success = storage->SerializeData(bookmarks_as_json);
    }
  }
}
