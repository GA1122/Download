void DataReductionProxySettings::RecordStartupState(
    ProxyStartupState state) const {
  UMA_HISTOGRAM_ENUMERATION(kUMAProxyStartupStateHistogram,
                            state,
                            PROXY_STARTUP_STATE_COUNT);
}
