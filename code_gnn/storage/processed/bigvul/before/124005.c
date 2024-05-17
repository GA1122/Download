BookmarkEventRouter::BookmarkEventRouter(BookmarkModel* model) : model_(model) {
  model_->AddObserver(this);
}
