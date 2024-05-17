void FakeCentral::UpdateFilter(
    std::unique_ptr<device::BluetoothDiscoveryFilter> discovery_filter,
    DiscoverySessionResultCallback callback) {
  if (!IsPresent()) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::BindOnce(
            std::move(callback),  true,
            device::UMABluetoothDiscoverySessionOutcome::ADAPTER_NOT_PRESENT));
    return;
  }

  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE,
      base::BindOnce(std::move(callback),  false,
                     device::UMABluetoothDiscoverySessionOutcome::SUCCESS));
}
