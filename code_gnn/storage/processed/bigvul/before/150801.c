void WebBluetoothServiceImpl::OnDescriptorWriteValueSuccess(
    RemoteDescriptorWriteValueCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  std::move(callback).Run(blink::mojom::WebBluetoothResult::SUCCESS);
}
