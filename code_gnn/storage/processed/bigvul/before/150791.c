void WebBluetoothServiceImpl::OnBluetoothScanningPromptEvent(
    BluetoothScanningPrompt::Event event,
    BluetoothDeviceScanningPromptController* prompt_controller) {
  DCHECK(!scanning_clients_.empty());

  auto client = scanning_clients_.end() - 1;

  DCHECK((*client)->prompt_controller() == prompt_controller);

  auto result = blink::mojom::WebBluetoothResult::SUCCESS;
  if (event == BluetoothScanningPrompt::Event::kAllow) {
    result = blink::mojom::WebBluetoothResult::SUCCESS;
    StoreAllowedScanOptions((*client)->scan_options());
  } else if (event == BluetoothScanningPrompt::Event::kBlock) {
    result = blink::mojom::WebBluetoothResult::SCANNING_BLOCKED;
    const url::Origin requesting_origin =
        render_frame_host_->GetLastCommittedOrigin();
    const url::Origin embedding_origin =
        web_contents()->GetMainFrame()->GetLastCommittedOrigin();
    GetContentClient()->browser()->BlockBluetoothScanning(
        web_contents()->GetBrowserContext(), requesting_origin,
        embedding_origin);
  } else if (event == BluetoothScanningPrompt::Event::kCanceled) {
    result = blink::mojom::WebBluetoothResult::PROMPT_CANCELED;
  } else {
    NOTREACHED();
  }

  (*client)->RunRequestScanningStartCallback(std::move(result));
  (*client)->set_prompt_controller(nullptr);
  if (event == BluetoothScanningPrompt::Event::kAllow) {
    (*client)->set_allow_send_event(true);
  } else if (event == BluetoothScanningPrompt::Event::kBlock) {
    scanning_clients_.clear();
    allowed_scan_filters_.clear();
    accept_all_advertisements_ = false;
  } else if (event == BluetoothScanningPrompt::Event::kCanceled) {
    scanning_clients_.erase(client);
  } else {
    NOTREACHED();
  }
}
