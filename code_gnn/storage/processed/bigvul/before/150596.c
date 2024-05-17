void DataReductionProxyIOData::DeleteBrowsingHistory(const base::Time start,
                                                     const base::Time end) {
  DCHECK(io_task_runner_->BelongsToCurrentThread());
  network_properties_manager_->DeleteHistory();
}
