void DataReductionProxySettings::OnServiceInitialized() {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (!deferred_initialization_)
    return;
  deferred_initialization_ = false;
  UpdateIOData(true);
  if (proxy_config_client_) {
    data_reduction_proxy_service_->SetCustomProxyConfigClient(
        std::move(proxy_config_client_));
  }
}
