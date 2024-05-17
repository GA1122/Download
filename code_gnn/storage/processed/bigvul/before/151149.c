bool InspectorNetworkAgent::CacheDisabled() {
  return state_->booleanProperty(NetworkAgentState::kNetworkAgentEnabled,
                                 false) &&
         state_->booleanProperty(NetworkAgentState::kCacheDisabled, false);
}
