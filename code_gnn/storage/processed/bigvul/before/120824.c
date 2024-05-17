void RecordPairingResult(BluetoothDevice::ConnectErrorCode error_code) {
  UMAPairingResult pairing_result;
  switch (error_code) {
    case BluetoothDevice::ERROR_INPROGRESS:
      pairing_result = UMA_PAIRING_RESULT_INPROGRESS;
      break;
    case BluetoothDevice::ERROR_FAILED:
      pairing_result = UMA_PAIRING_RESULT_FAILED;
      break;
    case BluetoothDevice::ERROR_AUTH_FAILED:
      pairing_result = UMA_PAIRING_RESULT_AUTH_FAILED;
      break;
    case BluetoothDevice::ERROR_AUTH_CANCELED:
      pairing_result = UMA_PAIRING_RESULT_AUTH_CANCELED;
      break;
    case BluetoothDevice::ERROR_AUTH_REJECTED:
      pairing_result = UMA_PAIRING_RESULT_AUTH_REJECTED;
      break;
    case BluetoothDevice::ERROR_AUTH_TIMEOUT:
      pairing_result = UMA_PAIRING_RESULT_AUTH_TIMEOUT;
      break;
    case BluetoothDevice::ERROR_UNSUPPORTED_DEVICE:
      pairing_result = UMA_PAIRING_RESULT_UNSUPPORTED_DEVICE;
      break;
    default:
      pairing_result = UMA_PAIRING_RESULT_UNKNOWN_ERROR;
  }

  UMA_HISTOGRAM_ENUMERATION("Bluetooth.PairingResult",
                            pairing_result,
                            UMA_PAIRING_RESULT_COUNT);
}
