void UsbChooserContext::OnDeviceAdded(
    device::mojom::UsbDeviceInfoPtr device_info) {
  DCHECK(device_info);

  for (auto& observer : observer_list_)
    observer.OnDeviceAdded(*device_info);
}
