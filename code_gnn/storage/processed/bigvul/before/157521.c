 void DataReductionProxyConfig::OnNewClientConfigFetched() {
   DCHECK(thread_checker_.CalledOnValidThread());
   ReloadConfig();
  network_properties_manager_->ResetWarmupURLFetchMetrics();
  FetchWarmupProbeURL();
}
