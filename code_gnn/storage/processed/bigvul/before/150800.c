void WebBluetoothServiceImpl::OnDescriptorWriteValueFailed(
    RemoteDescriptorWriteValueCallback callback,
    device::BluetoothRemoteGattService::GattErrorCode error_code) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  RecordDescriptorWriteValueOutcome(UMAGATTOperationOutcome::SUCCESS);
  std::move(callback).Run(TranslateGATTErrorAndRecord(
      error_code, UMAGATTOperation::DESCRIPTOR_WRITE));
}
