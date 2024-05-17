void BluetoothRemoteGATTCharacteristic::ReadValueCallback(
    ScriptPromiseResolver* resolver,
    mojom::blink::WebBluetoothResult result,
    const Optional<Vector<uint8_t>>& value) {
  if (!resolver->getExecutionContext() ||
      resolver->getExecutionContext()->isContextDestroyed())
    return;

  if (!getGatt()->RemoveFromActiveAlgorithms(resolver)) {
    resolver->reject(BluetoothRemoteGATTUtils::CreateDOMException(
        BluetoothRemoteGATTUtils::ExceptionType::kGATTServerDisconnected));
    return;
  }

  if (result == mojom::blink::WebBluetoothResult::SUCCESS) {
    DCHECK(value);
    DOMDataView* domDataView =
        BluetoothRemoteGATTUtils::ConvertWTFVectorToDataView(value.value());
    setValue(domDataView);
    resolver->resolve(domDataView);
  } else {
    resolver->reject(BluetoothError::take(resolver, result));
  }
}
