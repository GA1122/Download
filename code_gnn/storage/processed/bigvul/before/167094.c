BluetoothApiSocket* BluetoothSocketAsyncApiFunction::GetSocket(
    int api_resource_id) {
  return manager_->Get(extension_id(), api_resource_id);
}
