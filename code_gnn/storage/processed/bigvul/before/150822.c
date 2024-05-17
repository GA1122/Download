void WebBluetoothServiceImpl::RemoteServerDisconnect(
    const blink::WebBluetoothDeviceId& device_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (connected_devices_->IsConnectedToDeviceWithId(device_id)) {
    DVLOG(1) << "Disconnecting device: " << device_id.str();
    connected_devices_->CloseConnectionToDeviceWithId(device_id);
  }
}
