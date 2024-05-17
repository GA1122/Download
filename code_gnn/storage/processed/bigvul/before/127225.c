bool WorkerProcessLauncher::Core::OnMessageReceived(
    const IPC::Message& message) {
  DCHECK(caller_task_runner_->BelongsToCurrentThread());

  if (!ipc_enabled_)
    return false;

  return worker_delegate_->OnMessageReceived(message);
}
