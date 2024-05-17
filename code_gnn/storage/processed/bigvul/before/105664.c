void TreeView::RemoveObserverFromModel() {
  if (model_ && observer_added_) {
    model_->RemoveObserver(this);
    observer_added_ = false;
  }
}
