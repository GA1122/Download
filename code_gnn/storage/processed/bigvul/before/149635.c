void ResourcePrefetchPredictor::StartInitialization() {
  TRACE_EVENT0("browser", "ResourcePrefetchPredictor::StartInitialization");

  if (initialization_state_ != NOT_INITIALIZED)
    return;
  initialization_state_ = INITIALIZING;

  auto host_redirect_data = std::make_unique<RedirectDataMap>(
      tables_, tables_->host_redirect_table(), config_.max_hosts_to_track,
      base::TimeDelta::FromSeconds(config_.flush_data_to_disk_delay_seconds));
  auto origin_data = std::make_unique<OriginDataMap>(
      tables_, tables_->origin_table(), config_.max_hosts_to_track,
      base::TimeDelta::FromSeconds(config_.flush_data_to_disk_delay_seconds));

  auto task = base::BindOnce(InitializeOnDBSequence, host_redirect_data.get(),
                             origin_data.get());
  auto reply = base::BindOnce(
      &ResourcePrefetchPredictor::CreateCaches, weak_factory_.GetWeakPtr(),
      std::move(host_redirect_data), std::move(origin_data));

  tables_->GetTaskRunner()->PostTaskAndReply(FROM_HERE, std::move(task),
                                             std::move(reply));
}
