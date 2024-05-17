  void BluetoothDeviceChromeOS::ConfirmPairing() {
  if (!agent_.get() || confirmation_callback_.is_null())
//   if (!pairing_context_.get())
      return;
  
  confirmation_callback_.Run(SUCCESS);
  confirmation_callback_.Reset();
//   pairing_context_->ConfirmPairing();
  }