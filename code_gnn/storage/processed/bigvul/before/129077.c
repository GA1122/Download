bool DebuggerFunction::InitClientHost() {
  if (!InitAgentHost())
    return false;

  client_host_ = AttachedClientHosts::GetInstance()->Lookup(
      agent_host_.get(), GetExtension()->id());

  if (!client_host_) {
    FormatErrorMessage(keys::kNotAttachedError);
    return false;
  }
  return true;
}
