void FakeCentral::SimulateAdvertisementReceived(
    mojom::ScanResultPtr scan_result_ptr,
    SimulateAdvertisementReceivedCallback callback) {
  if (NumDiscoverySessions() == 0) {
    std::move(callback).Run();
    return;
  }
  auto* fake_peripheral = GetFakePeripheral(scan_result_ptr->device_address);
  const bool is_new_device = fake_peripheral == nullptr;
  if (is_new_device) {
    auto fake_peripheral_ptr =
        std::make_unique<FakePeripheral>(this, scan_result_ptr->device_address);
    fake_peripheral = fake_peripheral_ptr.get();
    auto pair = devices_.emplace(scan_result_ptr->device_address,
                                 std::move(fake_peripheral_ptr));
    DCHECK(pair.second);
  }

  auto& scan_record = scan_result_ptr->scan_record;
  auto uuids = ValueOrDefault(std::move(scan_record->uuids));
  auto service_data = ValueOrDefault(std::move(scan_record->service_data));
  auto manufacturer_data = ToManufacturerDataMap(
      ValueOrDefault(std::move(scan_record->manufacturer_data)));

  for (auto& observer : observers_) {
    observer.DeviceAdvertisementReceived(
        scan_result_ptr->device_address, scan_record->name, scan_record->name,
        scan_result_ptr->rssi, scan_record->tx_power->value,
        base::nullopt,  
        uuids, service_data, manufacturer_data);
  }

  fake_peripheral->SetName(std::move(scan_record->name));
  fake_peripheral->UpdateAdvertisementData(
      scan_result_ptr->rssi, base::nullopt  , uuids,
      scan_record->tx_power->has_value
          ? base::make_optional(scan_record->tx_power->value)
          : base::nullopt,
      service_data, manufacturer_data);

  if (is_new_device) {
    for (auto& observer : observers_) {
      observer.DeviceAdded(this, fake_peripheral);
    }
  } else {
    for (auto& observer : observers_) {
      observer.DeviceChanged(this, fake_peripheral);
    }
  }

   std::move(callback).Run();
 }
