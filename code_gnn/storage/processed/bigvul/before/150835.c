blink::mojom::WebBluetoothResult TranslateConnectErrorAndRecord(
    device::BluetoothDevice::ConnectErrorCode error_code) {
  switch (error_code) {
    case device::BluetoothDevice::ERROR_UNKNOWN:
      RecordConnectGATTOutcome(UMAConnectGATTOutcome::UNKNOWN);
      return blink::mojom::WebBluetoothResult::CONNECT_UNKNOWN_ERROR;
    case device::BluetoothDevice::ERROR_INPROGRESS:
      RecordConnectGATTOutcome(UMAConnectGATTOutcome::IN_PROGRESS);
      return blink::mojom::WebBluetoothResult::CONNECT_ALREADY_IN_PROGRESS;
    case device::BluetoothDevice::ERROR_FAILED:
      RecordConnectGATTOutcome(UMAConnectGATTOutcome::FAILED);
      return blink::mojom::WebBluetoothResult::CONNECT_UNKNOWN_FAILURE;
    case device::BluetoothDevice::ERROR_AUTH_FAILED:
      RecordConnectGATTOutcome(UMAConnectGATTOutcome::AUTH_FAILED);
      return blink::mojom::WebBluetoothResult::CONNECT_AUTH_FAILED;
    case device::BluetoothDevice::ERROR_AUTH_CANCELED:
      RecordConnectGATTOutcome(UMAConnectGATTOutcome::AUTH_CANCELED);
      return blink::mojom::WebBluetoothResult::CONNECT_AUTH_CANCELED;
    case device::BluetoothDevice::ERROR_AUTH_REJECTED:
      RecordConnectGATTOutcome(UMAConnectGATTOutcome::AUTH_REJECTED);
      return blink::mojom::WebBluetoothResult::CONNECT_AUTH_REJECTED;
    case device::BluetoothDevice::ERROR_AUTH_TIMEOUT:
      RecordConnectGATTOutcome(UMAConnectGATTOutcome::AUTH_TIMEOUT);
      return blink::mojom::WebBluetoothResult::CONNECT_AUTH_TIMEOUT;
    case device::BluetoothDevice::ERROR_UNSUPPORTED_DEVICE:
      RecordConnectGATTOutcome(UMAConnectGATTOutcome::UNSUPPORTED_DEVICE);
      return blink::mojom::WebBluetoothResult::CONNECT_UNSUPPORTED_DEVICE;
    case device::BluetoothDevice::NUM_CONNECT_ERROR_CODES:
      NOTREACHED();
      return blink::mojom::WebBluetoothResult::CONNECT_UNKNOWN_FAILURE;
  }
  NOTREACHED();
  return blink::mojom::WebBluetoothResult::CONNECT_UNKNOWN_FAILURE;
}
