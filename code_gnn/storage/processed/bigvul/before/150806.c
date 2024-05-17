void WebBluetoothServiceImpl::OnStartNotifySessionFailed(
    RemoteCharacteristicStartNotificationsCallback callback,
    device::BluetoothRemoteGattService::GattErrorCode error_code) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  std::move(callback).Run(TranslateGATTErrorAndRecord(
      error_code, UMAGATTOperation::START_NOTIFICATIONS));
}
