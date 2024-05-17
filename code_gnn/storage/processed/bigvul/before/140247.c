BluetoothRemoteGATTService::BluetoothRemoteGATTService(
    mojom::blink::WebBluetoothRemoteGATTServicePtr service,
    bool isPrimary,
    const String& deviceInstanceId,
    BluetoothDevice* device)
    : m_service(std::move(service)),
      m_isPrimary(isPrimary),
      m_deviceInstanceId(deviceInstanceId),
      m_device(device) {}
