void WebBluetoothServiceImpl::RunPendingPrimaryServicesRequests(
    device::BluetoothDevice* device) {
  const std::string& device_address = device->GetAddress();

  auto iter = pending_primary_services_requests_.find(device_address);
  if (iter == pending_primary_services_requests_.end()) {
    return;
  }
  std::vector<PrimaryServicesRequestCallback> requests =
      std::move(iter->second);
  pending_primary_services_requests_.erase(iter);

  for (PrimaryServicesRequestCallback& request : requests) {
    std::move(request).Run(device);
  }

  DCHECK(!base::Contains(pending_primary_services_requests_, device_address));
}
