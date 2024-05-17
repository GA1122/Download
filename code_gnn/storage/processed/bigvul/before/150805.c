void WebBluetoothServiceImpl::OnStartDiscoverySession(
    blink::mojom::WebBluetoothScanClientAssociatedPtr client,
    blink::mojom::WebBluetoothRequestLEScanOptionsPtr options,
    std::unique_ptr<device::BluetoothDiscoverySession> session) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!discovery_session_);

  discovery_session_ = std::move(session);

  if (AreScanFiltersAllowed(options->filters)) {
    auto scanning_client = std::make_unique<ScanningClient>(
        std::move(client), std::move(options), std::move(discovery_callback_),
        nullptr);
    scanning_client->RunRequestScanningStartCallback(
        blink::mojom::WebBluetoothResult::SUCCESS);
    scanning_client->set_allow_send_event(true);
    scanning_clients_.push_back(std::move(scanning_client));
    return;
  }

  device_scanning_prompt_controller_ =
      std::make_unique<BluetoothDeviceScanningPromptController>(
          this, render_frame_host_);

  scanning_clients_.push_back(std::make_unique<ScanningClient>(
      std::move(client), std::move(options), std::move(discovery_callback_),
      device_scanning_prompt_controller_.get()));
  device_scanning_prompt_controller_->ShowPermissionPrompt();
}
