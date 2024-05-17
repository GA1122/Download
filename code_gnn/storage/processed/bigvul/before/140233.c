ScriptPromise BluetoothRemoteGATTCharacteristic::stopNotifications(
    ScriptState* scriptState) {
  if (!getGatt()->connected()) {
    return ScriptPromise::rejectWithDOMException(
        scriptState,
        BluetoothRemoteGATTUtils::CreateDOMException(
            BluetoothRemoteGATTUtils::ExceptionType::kGATTServerNotConnected));
  }

  if (!getGatt()->device()->isValidCharacteristic(
          m_characteristic->instance_id)) {
    return ScriptPromise::rejectWithDOMException(
        scriptState,
        BluetoothRemoteGATTUtils::CreateDOMException(
            BluetoothRemoteGATTUtils::ExceptionType::kInvalidCharacteristic));
  }

  ScriptPromiseResolver* resolver = ScriptPromiseResolver::create(scriptState);
  ScriptPromise promise = resolver->promise();
  getGatt()->AddToActiveAlgorithms(resolver);

  mojom::blink::WebBluetoothService* service = m_device->bluetooth()->service();
  service->RemoteCharacteristicStopNotifications(
      m_characteristic->instance_id,
      convertToBaseCallback(
          WTF::bind(&BluetoothRemoteGATTCharacteristic::NotificationsCallback,
                    wrapPersistent(this), wrapPersistent(resolver),
                    mojom::blink::WebBluetoothResult::SUCCESS)));
  return promise;
}
