DataReductionProxySettings::GetDailyContentLengths(const char* pref_name) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(data_reduction_proxy_service_->compression_stats());
  return data_reduction_proxy_service_->compression_stats()->
      GetDailyContentLengths(pref_name);
}
