ScriptPromise BluetoothRemoteGATTService::getCharacteristics(
    ScriptState* scriptState,
    ExceptionState&) {
  return getCharacteristicsImpl(
      scriptState, mojom::blink::WebBluetoothGATTQueryQuantity::MULTIPLE);
}
