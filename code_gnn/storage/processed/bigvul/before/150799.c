void WebBluetoothServiceImpl::OnDescriptorReadValueSuccess(
    RemoteDescriptorReadValueCallback callback,
    const std::vector<uint8_t>& value) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  RecordDescriptorReadValueOutcome(UMAGATTOperationOutcome::SUCCESS);
  std::move(callback).Run(blink::mojom::WebBluetoothResult::SUCCESS, value);
}
