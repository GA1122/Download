BluetoothAllowedDevices& WebBluetoothServiceImpl::allowed_devices() {
  StoragePartitionImpl* partition = static_cast<StoragePartitionImpl*>(
      BrowserContext::GetDefaultStoragePartition(
          web_contents()->GetBrowserContext()));
  scoped_refptr<BluetoothAllowedDevicesMap> allowed_devices_map =
      partition->GetBluetoothAllowedDevicesMap();
  return allowed_devices_map->GetOrCreateAllowedDevices(GetOrigin());
}
