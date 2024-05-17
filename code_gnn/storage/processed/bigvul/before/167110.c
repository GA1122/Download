void BluetoothSocketAsyncApiFunction::RemoveSocket(int api_resource_id) {
  manager_->Remove(extension_id(), api_resource_id);
}
