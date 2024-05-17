BluetoothAdapter::GetMergedDiscoveryFilter() const {
  auto result =
      std::make_unique<BluetoothDiscoveryFilter>(BLUETOOTH_TRANSPORT_DUAL);
  bool first_merge = true;

  for (auto* iter : discovery_sessions_) {
    if (!iter->IsActive())
      continue;

    const BluetoothDiscoveryFilter* curr_filter = iter->GetDiscoveryFilter();

    if (first_merge) {
      first_merge = false;
      if (curr_filter) {
        result->CopyFrom(*curr_filter);
      }
      continue;
    }
    result = BluetoothDiscoveryFilter::Merge(result.get(), curr_filter);
  }
  return result;
}
