void DataReductionProxyIOData::OnEffectiveConnectionTypeChanged(
    net::EffectiveConnectionType type) {
  DCHECK(io_task_runner_->BelongsToCurrentThread());
  effective_connection_type_ = type;
  UpdateCustomProxyConfig();
}
