ScriptPromise BluetoothRemoteGATTServer::getPrimaryServices(
    ScriptState* scriptState,
    const StringOrUnsignedLong& service,
    ExceptionState& exceptionState) {
  String serviceUUID = BluetoothUUID::getService(service, exceptionState);
  if (exceptionState.hadException())
    return exceptionState.reject(scriptState);

  return getPrimaryServicesImpl(
      scriptState, mojom::blink::WebBluetoothGATTQueryQuantity::MULTIPLE,
      serviceUUID);
}
