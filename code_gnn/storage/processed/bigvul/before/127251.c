void WtsConsoleSessionProcessDriver::OnSessionDetached() {
  DCHECK(caller_task_runner_->BelongsToCurrentThread());
  DCHECK(launcher_.get() != NULL);

  launcher_.reset();
}
