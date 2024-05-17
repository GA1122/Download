int64_t DataReductionProxySettings::GetTotalHttpContentLengthSaved() {
  DCHECK(thread_checker_.CalledOnValidThread());
  return data_reduction_proxy_service_->compression_stats()
             ->GetHttpOriginalContentLength() -
         data_reduction_proxy_service_->compression_stats()
             ->GetHttpReceivedContentLength();
}
