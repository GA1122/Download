void BluetoothOptionsHandler::DefaultAdapterChanged(
    chromeos::BluetoothAdapter* adapter) {
  std::string old_default_adapter_id = default_adapter_id_;

  if (adapter == NULL) {
    default_adapter_id_.clear();
    VLOG(2) << "DefaultAdapterChanged: no default bluetooth adapter";
  } else {
    default_adapter_id_ = adapter->Id();
    VLOG(2) << "DefaultAdapterChanged: " << default_adapter_id_;
  }

  if (default_adapter_id_ == old_default_adapter_id) {
    return;
  }

  if (adapter != NULL) {
    adapter->AddObserver(this);
  }

}
