    ~AutomationProviderBookmarkModelObserver() {
  model_->RemoveObserver(this);
}
