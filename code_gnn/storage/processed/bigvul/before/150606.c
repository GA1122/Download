void DataReductionProxyIOData::SetDataReductionProxyConfiguration(
    const std::string& serialized_config) {
  DCHECK(io_task_runner_->BelongsToCurrentThread());
  if (config_client_)
    config_client_->ApplySerializedConfig(serialized_config);
}
