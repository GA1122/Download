void BluetoothRemoteGATTService::GetCharacteristicsCallback(
    const String& serviceInstanceId,
    mojom::blink::WebBluetoothGATTQueryQuantity quantity,
    ScriptPromiseResolver* resolver,
    mojom::blink::WebBluetoothResult result,
    Optional<Vector<mojom::blink::WebBluetoothRemoteGATTCharacteristicPtr>>
        characteristics) {
  if (!resolver->getExecutionContext() ||
      resolver->getExecutionContext()->isContextDestroyed())
    return;

  if (!device()->gatt()->RemoveFromActiveAlgorithms(resolver)) {
    resolver->reject(
        DOMException::create(NetworkError, kGATTServerDisconnected));
    return;
  }

  if (result == mojom::blink::WebBluetoothResult::SUCCESS) {
    DCHECK(characteristics);

    if (quantity == mojom::blink::WebBluetoothGATTQueryQuantity::SINGLE) {
      DCHECK_EQ(1u, characteristics->size());
      resolver->resolve(device()->getOrCreateRemoteGATTCharacteristic(
          resolver->getExecutionContext(),
          std::move(characteristics.value()[0]), this));
      return;
    }

    HeapVector<Member<BluetoothRemoteGATTCharacteristic>> gattCharacteristics;
    gattCharacteristics.reserveInitialCapacity(characteristics->size());
    for (auto& characteristic : characteristics.value()) {
      gattCharacteristics.push_back(
          device()->getOrCreateRemoteGATTCharacteristic(
              resolver->getExecutionContext(), std::move(characteristic),
              this));
    }
    resolver->resolve(gattCharacteristics);
  } else {
    resolver->reject(BluetoothError::take(resolver, result));
  }
}
