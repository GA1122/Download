  GATTNotifySessionAndCharacteristicClient(
      std::unique_ptr<device::BluetoothGattNotifySession> session,
      blink::mojom::WebBluetoothCharacteristicClientAssociatedPtr client)
      : gatt_notify_session(std::move(session)),
        characteristic_client(std::move(client)) {}
