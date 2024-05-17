void WebBluetoothServiceImpl::OnCharacteristicWriteValueSuccess(
    RemoteCharacteristicWriteValueCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  RecordCharacteristicWriteValueOutcome(UMAGATTOperationOutcome::SUCCESS);
  std::move(callback).Run(blink::mojom::WebBluetoothResult::SUCCESS);
}
