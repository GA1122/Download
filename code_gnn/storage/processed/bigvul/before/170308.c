void UsbChooserContext::GrantDevicePermission(
    const GURL& requesting_origin,
    const GURL& embedding_origin,
    const device::mojom::UsbDeviceInfo& device_info) {
  if (CanStorePersistentEntry(device_info)) {
    GrantObjectPermission(requesting_origin, embedding_origin,
                          std::make_unique<base::DictionaryValue>(
                              DeviceInfoToDictValue(device_info)));
  } else {
    ephemeral_devices_[std::make_pair(requesting_origin, embedding_origin)]
        .insert(device_info.guid);
    if (!base::ContainsKey(ephemeral_dicts_, device_info.guid)) {
      ephemeral_dicts_.insert(
          std::make_pair(device_info.guid, DeviceInfoToDictValue(device_info)));
    }
  }
}
