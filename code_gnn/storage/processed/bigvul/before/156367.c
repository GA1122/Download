void ExtensionDevToolsClientHost::RespondDetachedToPendingRequests() {
  for (const auto& it : pending_requests_)
    it.second->SendDetachedError();
  pending_requests_.clear();
}
