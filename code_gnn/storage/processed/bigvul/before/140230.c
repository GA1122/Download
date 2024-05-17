ScriptPromise BluetoothRemoteGATTCharacteristic::readValue(
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
  service->RemoteCharacteristicReadValue(
      m_characteristic->instance_id,
      convertToBaseCallback(
          WTF::bind(&BluetoothRemoteGATTCharacteristic::ReadValueCallback,
                    wrapPersistent(this), wrapPersistent(resolver))));

  return promise;
}
