DataReductionProxyIOData::GetEffectiveConnectionType() const {
  DCHECK(io_task_runner_->BelongsToCurrentThread());
  return effective_connection_type_;
}
