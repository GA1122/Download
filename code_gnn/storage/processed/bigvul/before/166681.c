void ThreadHeap::DecommitCallbackStacks() {
  marking_worklist_.reset(nullptr);
  not_fully_constructed_worklist_.reset(nullptr);
  weak_callback_worklist_.reset(nullptr);
  ephemeron_callbacks_.clear();
}
