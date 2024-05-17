void DataReductionProxyIOData::SetDataReductionProxyService(
    base::WeakPtr<DataReductionProxyService> data_reduction_proxy_service,
    const std::string& user_agent) {
  DCHECK(ui_task_runner_->BelongsToCurrentThread());
  service_ = data_reduction_proxy_service;
  url_loader_factory_info_ = service_->url_loader_factory_info();
  if (io_task_runner_->BelongsToCurrentThread()) {
    InitializeOnIOThread(user_agent);
    return;
  }
  io_task_runner_->PostTask(
      FROM_HERE, base::BindOnce(&DataReductionProxyIOData::InitializeOnIOThread,
                                base::Unretained(this), user_agent));
}
