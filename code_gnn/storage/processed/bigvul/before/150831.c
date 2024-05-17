void WebBluetoothServiceImpl::ScanningClient::RunRequestScanningStartCallback(
    blink::mojom::WebBluetoothResult result) {
  if (result == blink::mojom::WebBluetoothResult::SUCCESS) {
    auto scanning_result =
        blink::mojom::RequestScanningStartResult::NewOptions(options_.Clone());
    std::move(callback_).Run(std::move(scanning_result));
  } else if (result == blink::mojom::WebBluetoothResult::SCANNING_BLOCKED ||
             result == blink::mojom::WebBluetoothResult::PROMPT_CANCELED) {
    auto scanning_result =
        blink::mojom::RequestScanningStartResult::NewErrorResult(result);
    std::move(callback_).Run(std::move(scanning_result));
  } else {
    NOTREACHED();
  }
}
