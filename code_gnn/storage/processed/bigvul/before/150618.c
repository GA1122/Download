void DataReductionProxyIOData::UpdateDataUseForHost(int64_t network_bytes,
                                                    int64_t original_bytes,
                                                    const std::string& host) {
  DCHECK(io_task_runner_->BelongsToCurrentThread());

  ui_task_runner_->PostTask(
      FROM_HERE,
      base::BindOnce(&DataReductionProxyService::UpdateDataUseForHost, service_,
                     network_bytes, original_bytes, host));
}
