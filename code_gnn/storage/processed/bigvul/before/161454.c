Response TargetHandler::AttachToTarget(const std::string& target_id,
                                       std::string* out_session_id) {
  scoped_refptr<DevToolsAgentHost> agent_host =
      DevToolsAgentHost::GetForId(target_id);
  if (!agent_host)
    return Response::InvalidParams("No target with given id found");
  *out_session_id = Session::Attach(this, agent_host.get(), false);
  return Response::OK();
}
