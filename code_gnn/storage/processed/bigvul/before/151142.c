void InspectorNetworkAgent::ApplyUserAgentOverride(String* user_agent) {
  String user_agent_override;
  state_->getString(NetworkAgentState::kUserAgentOverride,
                    &user_agent_override);
  if (!user_agent_override.IsEmpty())
    *user_agent = user_agent_override;
}
