WebBluetoothServiceImpl::ScanningClient::ScanningClient(
    blink::mojom::WebBluetoothScanClientAssociatedPtr client,
    blink::mojom::WebBluetoothRequestLEScanOptionsPtr options,
    RequestScanningStartCallback callback,
    BluetoothDeviceScanningPromptController* prompt_controller)
    : client_(std::move(client)),
      options_(std::move(options)),
      callback_(std::move(callback)),
      prompt_controller_(prompt_controller) {
  DCHECK(options_->filters.has_value() || options_->accept_all_advertisements);
  client_.set_connection_error_handler(base::BindRepeating(
      &ScanningClient::DisconnectionHandler, base::Unretained(this)));
}
