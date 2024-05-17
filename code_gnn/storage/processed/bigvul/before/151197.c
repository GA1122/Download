void InspectorNetworkAgent::Restore() {
  if (state_->booleanProperty(NetworkAgentState::kNetworkAgentEnabled, false)) {
    Enable(state_->integerProperty(NetworkAgentState::kTotalBufferSize,
                                   g_maximum_total_buffer_size),
           state_->integerProperty(NetworkAgentState::kResourceBufferSize,
                                   g_maximum_resource_buffer_size));
  }
}
