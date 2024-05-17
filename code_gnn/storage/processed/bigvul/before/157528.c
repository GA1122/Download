bool TestDataReductionProxyConfig::IsFetchInFlight() const {
  if (fetch_in_flight_)
    return fetch_in_flight_.value();
  return DataReductionProxyConfig::IsFetchInFlight();
}
