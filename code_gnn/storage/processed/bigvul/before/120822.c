void ParseModalias(const dbus::ObjectPath& object_path,
                   uint16 *vendor_id,
                   uint16 *product_id,
                   uint16 *device_id) {
  chromeos::BluetoothDeviceClient::Properties* properties =
      chromeos::DBusThreadManager::Get()->GetBluetoothDeviceClient()->
          GetProperties(object_path);
  DCHECK(properties);

  std::string modalias = properties->modalias.value();
  if (StartsWithASCII(modalias, "usb:", false) && modalias.length() == 19) {
    if (modalias[4] == 'v' && vendor_id != NULL) {
      uint64 component = 0;
      base::HexStringToUInt64(modalias.substr(5, 4), &component);
      *vendor_id = component;
    }

    if (modalias[9] == 'p' && product_id != NULL) {
      uint64 component = 0;
      base::HexStringToUInt64(modalias.substr(10, 4), &component);
      *product_id = component;
    }

    if (modalias[14] == 'd' && device_id != NULL) {
      uint64 component = 0;
      base::HexStringToUInt64(modalias.substr(15, 4), &component);
      *device_id = component;
    }
  }
}
