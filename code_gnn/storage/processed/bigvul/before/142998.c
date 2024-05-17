void BaseAudioContext::Clear() {
  destination_node_.Clear();
  GetDeferredTaskHandler().ClearHandlersToBeDeleted();
  is_cleared_ = true;
}
