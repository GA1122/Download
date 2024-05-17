bool ResourceDispatcherHostImpl::Send(IPC::Message* message) {
  delete message;
  return false;
}
