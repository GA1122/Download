QueryManager::Query::~Query() {
  RunCallbacks();
  if (manager_) {
    manager_->StopTracking(this);
    manager_ = NULL;
  }
}
