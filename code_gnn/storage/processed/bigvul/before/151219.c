Response InspectorNetworkAgent::setCacheDisabled(bool cache_disabled) {
  state_->setBoolean(NetworkAgentState::kCacheDisabled, cache_disabled);
  if (cache_disabled && IsMainThread())
    GetMemoryCache()->EvictResources();
  return Response::OK();
}
