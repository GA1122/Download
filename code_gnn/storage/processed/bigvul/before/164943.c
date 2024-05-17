GlobalRequestID ResourceDispatcherHostImpl::MakeGlobalRequestID() {
  return GlobalRequestID(ChildProcessHost::kInvalidUniqueID, MakeRequestID());
}
