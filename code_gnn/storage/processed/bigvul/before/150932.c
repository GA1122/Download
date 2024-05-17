void FakeCentral::StopScan(DiscoverySessionResultCallback callback) {
  if (!IsPresent()) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::BindOnce(
            std::move(callback),  false,
            device::UMABluetoothDiscoverySessionOutcome::ADAPTER_NOT_PRESENT));
    return;
  }

  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE,
      base::BindOnce(
          std::move(callback),  false,
          device::UMABluetoothDiscoverySessionOutcome::ADAPTER_NOT_PRESENT));
}
