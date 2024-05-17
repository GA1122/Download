void SharedWorkerDevToolsAgentHost::WorkerReadyForInspection() {
  DCHECK_EQ(WORKER_NOT_READY, state_);
  DCHECK(worker_host_);
  state_ = WORKER_READY;
  for (DevToolsSession* session : sessions())
    session->AttachToAgent(EnsureAgent());
}
