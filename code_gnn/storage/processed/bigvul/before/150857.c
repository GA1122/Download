void BluetoothAdapter::MaybeUpdateFilter(
    std::unique_ptr<BluetoothDiscoveryFilter> discovery_filter,
    DiscoverySessionResultCallback callback) {
  if (discovery_filter->Equals(current_discovery_filter_)) {
    std::move(callback).Run( false,
                            UMABluetoothDiscoverySessionOutcome::SUCCESS);
    return;
  }

  UpdateFilter(std::move(discovery_filter), std::move(callback));
}
