void WtsConsoleSessionProcessDriver::DoStop() {
  DCHECK(caller_task_runner_->BelongsToCurrentThread());

  launcher_.reset();
  CompleteStopping();
}
