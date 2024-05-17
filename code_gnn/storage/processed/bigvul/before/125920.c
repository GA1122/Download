void AutomationProviderBookmarkModelObserver::BookmarkModelBeingDeleted(
    BookmarkModel* model) {
  ReplyAndDelete(false);
}
