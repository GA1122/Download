bool DetachDebuggerFunction::RunImpl() {
  if (!InitClientHost())
    return false;

  client_host_->Close();
  SendResponse(true);
  return true;
}
