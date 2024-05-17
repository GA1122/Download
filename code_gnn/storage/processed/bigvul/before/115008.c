void TestingAutomationProvider::RemoveBookmark(int handle,
                                               int64 id,
                                               bool* success) {
  if (browser_tracker_->ContainsHandle(handle)) {
    Browser* browser = browser_tracker_->GetResource(handle);
    if (browser) {
      BookmarkModel* model = browser->profile()->GetBookmarkModel();
      if (!model->IsLoaded()) {
        *success = false;
        return;
      }
      const BookmarkNode* node = model->GetNodeByID(id);
      DCHECK(node);
      if (node) {
        const BookmarkNode* parent = node->parent();
        DCHECK(parent);
        model->Remove(parent, parent->GetIndexOf(node));
        *success = true;
      }
    }
  }
  *success = false;
}
