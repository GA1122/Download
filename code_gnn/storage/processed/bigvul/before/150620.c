void DataReductionProxyIOData::UpdateThrottleConfig() {
  if (drp_throttle_config_observers_.empty())
    return;

  auto config = CreateThrottleConfig();

  drp_throttle_config_observers_.ForAllPtrs(
      [&config](mojom::DataReductionProxyThrottleConfigObserver* observer) {
        observer->OnThrottleConfigChanged(config->Clone());
      });
}
