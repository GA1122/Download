BubbleManager::~BubbleManager() {
  FinalizePendingRequests();
}