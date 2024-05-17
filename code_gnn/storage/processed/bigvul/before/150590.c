void DataReductionProxyIOData::AddThrottleConfigObserver(
    mojom::DataReductionProxyThrottleConfigObserverPtr observer) {
  observer->OnThrottleConfigChanged(CreateThrottleConfig());
  drp_throttle_config_observers_.AddPtr(std::move(observer));
}
