void DataReductionProxySettings::UpdateIOData(bool at_startup) {
  data_reduction_proxy_service_->SetProxyPrefs(IsDataReductionProxyEnabled(),
                                               at_startup);
}
