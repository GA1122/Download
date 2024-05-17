void UsbChooserContext::GetDevices(
    device::mojom::UsbDeviceManager::GetDevicesCallback callback) {
  EnsureConnectionWithDeviceManager();
  device_manager_->GetDevices(nullptr, std::move(callback));
}
