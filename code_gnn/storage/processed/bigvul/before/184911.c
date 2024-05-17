  void BluetoothDeviceChromeOS::CancelPairing() {
  if (!RunPairingCallbacks(CANCELLED)) {
//   if (!pairing_context_.get() || !pairing_context_->CancelPairing()) {
//     VLOG(1) << object_path_.value() << ": No pairing context or callback. "
//             << "Sending explicit cancel";
      DBusThreadManager::Get()->GetBluetoothDeviceClient()->
          CancelPairing(
              object_path_,
             base::Bind(&base::DoNothing),
             base::Bind(&BluetoothDeviceChromeOS::OnCancelPairingError,
                         weak_ptr_factory_.GetWeakPtr()));
  
    UnregisterAgent();
//      
//      
//     pairing_context_.reset();
    }
  }