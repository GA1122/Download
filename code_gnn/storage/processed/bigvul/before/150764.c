void BluetoothDeviceChooserController::StopDeviceDiscovery() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (scanning_start_time_) {
    RecordScanningDuration(base::TimeTicks::Now() -
                           scanning_start_time_.value());
    scanning_start_time_.reset();
  }

  StopDiscoverySession(std::move(discovery_session_));
  if (chooser_) {
    chooser_->ShowDiscoveryState(BluetoothChooser::DiscoveryState::IDLE);
  }
}
