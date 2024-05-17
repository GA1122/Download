void TreeView::AddObserverToModel() {
  if (model_ && !observer_added_) {
    model_->AddObserver(this);
    observer_added_ = true;
  }
}
