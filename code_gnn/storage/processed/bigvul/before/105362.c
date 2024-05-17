AutofillManager::~AutofillManager() {
  download_manager_.SetObserver(NULL);
}
