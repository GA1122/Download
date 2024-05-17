void BluetoothRemoteGATTCharacteristic::WriteValueCallback(
    ScriptPromiseResolver* resolver,
    const Vector<uint8_t>& value,
    mojom::blink::WebBluetoothResult result) {
  if (!resolver->getExecutionContext() ||
      resolver->getExecutionContext()->isContextDestroyed())
    return;

  if (!getGatt()->RemoveFromActiveAlgorithms(resolver)) {
    resolver->reject(BluetoothRemoteGATTUtils::CreateDOMException(
        BluetoothRemoteGATTUtils::ExceptionType::kGATTServerDisconnected));
    return;
  }

  if (result == mojom::blink::WebBluetoothResult::SUCCESS) {
    setValue(BluetoothRemoteGATTUtils::ConvertWTFVectorToDataView(value));
    resolver->resolve();
  } else {
    resolver->reject(BluetoothError::take(resolver, result));
  }
}
