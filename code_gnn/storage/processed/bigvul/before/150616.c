void DataReductionProxyIOData::UpdateContentLengths(
    int64_t data_used,
    int64_t original_size,
    bool data_reduction_proxy_enabled,
    DataReductionProxyRequestType request_type,
    const std::string& mime_type,
    bool is_user_traffic,
    data_use_measurement::DataUseUserData::DataUseContentType content_type,
    int32_t service_hash_code) {
  DCHECK(io_task_runner_->BelongsToCurrentThread());

  ui_task_runner_->PostTask(
      FROM_HERE,
      base::BindRepeating(&DataReductionProxyService::UpdateContentLengths,
                          service_, data_used, original_size,
                          data_reduction_proxy_enabled, request_type, mime_type,
                          is_user_traffic, content_type, service_hash_code));
}
