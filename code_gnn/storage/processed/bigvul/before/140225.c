ScriptPromise BluetoothRemoteGATTCharacteristic::getDescriptor(
    ScriptState* scriptState,
    const StringOrUnsignedLong& descriptorUUID,
    ExceptionState& exceptionState) {
  String descriptor =
      BluetoothUUID::getDescriptor(descriptorUUID, exceptionState);
  if (exceptionState.hadException())
    return exceptionState.reject(scriptState);

  return getDescriptorsImpl(scriptState,
                            mojom::blink::WebBluetoothGATTQueryQuantity::SINGLE,
                            descriptor);
}
