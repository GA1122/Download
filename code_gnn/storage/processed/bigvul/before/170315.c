void UsbChooserContext::OnDeviceRemoved(
    device::mojom::UsbDeviceInfoPtr device_info) {
  DCHECK(device_info);

  for (auto& observer : observer_list_)
    observer.OnDeviceRemoved(*device_info);

  for (auto& map_entry : ephemeral_devices_)
    map_entry.second.erase(device_info->guid);

  ephemeral_dicts_.erase(device_info->guid);
}
