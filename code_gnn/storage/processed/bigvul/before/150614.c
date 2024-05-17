void DataReductionProxyIOData::ShutdownOnUIThread() {
  DCHECK(ui_task_runner_->BelongsToCurrentThread());
  network_properties_manager_->ShutdownOnUIThread();
}
