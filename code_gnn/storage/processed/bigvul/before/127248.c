bool WtsConsoleSessionProcessDriver::OnMessageReceived(
    const IPC::Message& message) {
  DCHECK(caller_task_runner_->BelongsToCurrentThread());

  return false;
}
