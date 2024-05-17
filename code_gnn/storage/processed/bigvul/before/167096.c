base::hash_set<int>* BluetoothSocketAsyncApiFunction::GetSocketIds() {
  return manager_->GetResourceIds(extension_id());
}
