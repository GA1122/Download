void WebBluetoothServiceImpl::OnGetDeviceFailed(
    RequestDeviceCallback callback,
    blink::mojom::WebBluetoothResult result) {
  std::move(callback).Run(result, nullptr  );
  device_chooser_controller_.reset();
}
