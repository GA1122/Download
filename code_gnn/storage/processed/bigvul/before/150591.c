void DataReductionProxyIOData::AddThrottleConfigObserverInfo(
    mojom::DataReductionProxyThrottleConfigObserverPtrInfo observer) {
  AddThrottleConfigObserver(
      mojom::DataReductionProxyThrottleConfigObserverPtr(std::move(observer)));
}
