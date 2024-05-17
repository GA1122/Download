 SharedWorkerDevToolsAgentHost::EnsureAgent() {
   DCHECK_EQ(WORKER_READY, state_);
  DCHECK(worker_host_);
  if (!agent_ptr_)
    worker_host_->BindDevToolsAgent(mojo::MakeRequest(&agent_ptr_));
  return agent_ptr_;
}
