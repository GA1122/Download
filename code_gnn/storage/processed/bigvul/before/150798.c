void WebBluetoothServiceImpl::OnDescriptorReadValueFailed(
    RemoteDescriptorReadValueCallback callback,
    device::BluetoothRemoteGattService::GattErrorCode error_code) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  std::move(callback).Run(TranslateGATTErrorAndRecord(
                              error_code, UMAGATTOperation::DESCRIPTOR_READ),
                          base::nullopt  );
}
