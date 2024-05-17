void WtsConsoleSessionProcessDriver::OnPermanentError() {
  DCHECK(caller_task_runner_->BelongsToCurrentThread());

  Stop();
}
