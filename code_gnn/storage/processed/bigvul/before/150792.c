void WebBluetoothServiceImpl::OnCharacteristicReadValueFailed(
    RemoteCharacteristicReadValueCallback callback,
    device::BluetoothRemoteGattService::GattErrorCode error_code) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  std::move(callback).Run(
      TranslateGATTErrorAndRecord(error_code,
                                  UMAGATTOperation::CHARACTERISTIC_READ),
      base::nullopt  );
}
