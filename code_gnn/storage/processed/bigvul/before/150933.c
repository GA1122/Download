device::BluetoothDevice::ManufacturerDataMap ToManufacturerDataMap(
    base::flat_map<uint8_t, std::vector<uint8_t>>&& map) {
  return device::BluetoothDevice::ManufacturerDataMap(
      std::make_move_iterator(map.begin()), std::make_move_iterator(map.end()));
}
