void DataReductionProxyIOData::OnRTTOrThroughputEstimatesComputed(
    base::TimeDelta http_rtt) {
  DCHECK(io_task_runner_->BelongsToCurrentThread());
  config_->OnRTTOrThroughputEstimatesComputed(http_rtt);
}
