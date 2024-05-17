void WebBluetoothServiceImpl::OnDiscoverySessionError() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  device_scanning_prompt_controller_.reset();

  auto result = blink::mojom::RequestScanningStartResult::NewErrorResult(
      blink::mojom::WebBluetoothResult::NO_BLUETOOTH_ADAPTER);
  std::move(discovery_callback_).Run(std::move(result));
}
