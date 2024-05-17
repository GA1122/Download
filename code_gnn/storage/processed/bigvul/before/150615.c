void DataReductionProxyIOData::StoreSerializedConfig(
    const std::string& serialized_config) {
  DCHECK(io_task_runner_->BelongsToCurrentThread());
  SetStringPref(prefs::kDataReductionProxyConfig, serialized_config);
  SetInt64Pref(prefs::kDataReductionProxyLastConfigRetrievalTime,
               (base::Time::Now() - base::Time()).InMicroseconds());
}
