void DataReductionProxyIOData::InitializeOnIOThread(
    const std::string& user_agent) {
  DCHECK(io_task_runner_->BelongsToCurrentThread());
  DCHECK(network_properties_manager_);

  DCHECK(url_loader_factory_info_);
  auto url_loader_factory = network::SharedURLLoaderFactory::Create(
      std::move(url_loader_factory_info_));

  config_->InitializeOnIOThread(
      url_loader_factory,
      base::BindRepeating(&DataReductionProxyIOData::CreateCustomProxyConfig,
                          base::Unretained(this), true),
      network_properties_manager_.get(), user_agent);
  if (config_client_)
    config_client_->InitializeOnIOThread(url_loader_factory);
  if (ui_task_runner_->BelongsToCurrentThread()) {
    service_->SetIOData(weak_factory_.GetWeakPtr());
    return;
  }
  ui_task_runner_->PostTask(
      FROM_HERE, base::BindOnce(&DataReductionProxyService::SetIOData, service_,
                                weak_factory_.GetWeakPtr()));
}
