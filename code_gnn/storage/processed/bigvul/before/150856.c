void BluetoothAdapter::MarkDiscoverySessionsAsInactive() {
  if (!discovery_callback_queue_.empty())
    NotifyDiscoveryError(std::move(discovery_callback_queue_));
  std::set<BluetoothDiscoverySession*> temp(discovery_sessions_);
  for (auto iter = temp.begin(); iter != temp.end(); ++iter) {
    (*iter)->MarkAsInactive();
    RemoveDiscoverySession(*iter, base::DoNothing(), base::DoNothing());
  }
}
