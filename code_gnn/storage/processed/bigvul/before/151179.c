void InspectorNetworkAgent::Enable(int total_buffer_size,
                                   int resource_buffer_size) {
  if (!GetFrontend())
    return;
  resources_data_->SetResourcesDataSizeLimits(total_buffer_size,
                                              resource_buffer_size);
  state_->setBoolean(NetworkAgentState::kNetworkAgentEnabled, true);
  state_->setInteger(NetworkAgentState::kTotalBufferSize, total_buffer_size);
  state_->setInteger(NetworkAgentState::kResourceBufferSize,
                     resource_buffer_size);
  instrumenting_agents_->addInspectorNetworkAgent(this);
}
