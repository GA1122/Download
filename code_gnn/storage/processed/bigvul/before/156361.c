bool DebuggerFunction::InitClientHost() {
  if (!InitAgentHost())
    return false;

  client_host_ = FindClientHost();
  if (!client_host_) {
    FormatErrorMessage(debugger_api_constants::kNotAttachedError);
    return false;
  }

  return true;
}
