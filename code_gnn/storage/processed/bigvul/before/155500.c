void DataReductionProxySettings::GetContentLengths(
    unsigned int days,
    int64_t* original_content_length,
    int64_t* received_content_length,
    int64_t* last_update_time) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(data_reduction_proxy_service_->compression_stats());

  data_reduction_proxy_service_->compression_stats()->GetContentLengths(
      days, original_content_length, received_content_length, last_update_time);
}
