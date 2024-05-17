void BluetoothRemoteGATTServer::disconnect(ScriptState* scriptState) {
  if (!m_connected)
    return;
  device()->cleanupDisconnectedDeviceAndFireEvent();
  m_device->bluetooth()->removeDevice(device()->id());
  mojom::blink::WebBluetoothService* service = m_device->bluetooth()->service();
  service->RemoteServerDisconnect(device()->id());
}
