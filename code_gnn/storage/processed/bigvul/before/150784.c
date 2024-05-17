  blink::mojom::WebBluetoothResult GetWebResult() const {
    switch (outcome) {
      case CacheQueryOutcome::SUCCESS:
      case CacheQueryOutcome::BAD_RENDERER:
        NOTREACHED();
        return blink::mojom::WebBluetoothResult::DEVICE_NO_LONGER_IN_RANGE;
      case CacheQueryOutcome::NO_DEVICE:
        return blink::mojom::WebBluetoothResult::DEVICE_NO_LONGER_IN_RANGE;
      case CacheQueryOutcome::NO_SERVICE:
        return blink::mojom::WebBluetoothResult::SERVICE_NO_LONGER_EXISTS;
      case CacheQueryOutcome::NO_CHARACTERISTIC:
        return blink::mojom::WebBluetoothResult::
            CHARACTERISTIC_NO_LONGER_EXISTS;
      case CacheQueryOutcome::NO_DESCRIPTOR:
        return blink::mojom::WebBluetoothResult::DESCRIPTOR_NO_LONGER_EXISTS;
    }
    NOTREACHED();
    return blink::mojom::WebBluetoothResult::DEVICE_NO_LONGER_IN_RANGE;
  }
