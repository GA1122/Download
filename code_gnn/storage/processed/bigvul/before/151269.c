Response InspectorPageAgent::disable() {
  enabled_ = false;
  state_->setBoolean(PageAgentState::kPageAgentEnabled, false);
  state_->remove(PageAgentState::kPageAgentScriptsToEvaluateOnLoad);
  script_to_evaluate_on_load_once_ = String();
  pending_script_to_evaluate_on_load_once_ = String();
  instrumenting_agents_->removeInspectorPageAgent(this);
  inspector_resource_content_loader_->Cancel(
      resource_content_loader_client_id_);

  stopScreencast();

  FinishReload();
  return Response::OK();
}
