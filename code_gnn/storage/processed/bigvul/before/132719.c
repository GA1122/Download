void ChromotingInstance::HandleDisconnect(const base::DictionaryValue& data) {
  DCHECK(plugin_task_runner_->BelongsToCurrentThread());
  Disconnect();
}
