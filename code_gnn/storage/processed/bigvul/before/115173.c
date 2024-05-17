 void BluetoothOptionsHandler::ValidateDefaultAdapter(
    chromeos::BluetoothAdapter* adapter) {
  if ((adapter == NULL && !default_adapter_id_.empty()) ||
      (adapter != NULL && default_adapter_id_ != adapter->Id())) {
    VLOG(1) << "unexpected default adapter change from \""
            << default_adapter_id_ << "\" to \"" << adapter->Id() << "\"";
    DefaultAdapterChanged(adapter);
  }
 }
