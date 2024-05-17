ScriptPromise BluetoothRemoteGATTCharacteristic::writeValue(
    ScriptState* scriptState,
    const DOMArrayPiece& value) {
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


  if (value.byteLength() > 512)
    return ScriptPromise::rejectWithDOMException(
        scriptState, DOMException::create(InvalidModificationError,
                                          "Value can't exceed 512 bytes."));

  Vector<uint8_t> valueVector;
  valueVector.append(value.bytes(), value.byteLength());

  ScriptPromiseResolver* resolver = ScriptPromiseResolver::create(scriptState);
  ScriptPromise promise = resolver->promise();
  getGatt()->AddToActiveAlgorithms(resolver);

  mojom::blink::WebBluetoothService* service = m_device->bluetooth()->service();
  service->RemoteCharacteristicWriteValue(
      m_characteristic->instance_id, valueVector,
      convertToBaseCallback(WTF::bind(
          &BluetoothRemoteGATTCharacteristic::WriteValueCallback,
          wrapPersistent(this), wrapPersistent(resolver), valueVector)));

  return promise;
}
