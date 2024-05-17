bool DebuggerFunction::InitClientHost() {
  if (!InitAgentHost())
    return false;

  client_host_ = FindClientHost();
  if (!client_host_) {
    FormatErrorMessage(keys::kNotAttachedError);
    return false;
  }

  return true;
}
