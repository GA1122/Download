ChromotingInstance::~ChromotingInstance() {
  DCHECK(plugin_task_runner_->BelongsToCurrentThread());

  Disconnect();

  UnregisterLoggingInstance();

  plugin_task_runner_->Quit();

  plugin_task_runner_->DetachAndRunShutdownLoop();

  context_.Stop();
}
