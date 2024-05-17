void WebBluetoothServiceImpl::OnStopNotifySessionComplete(
    const std::string& characteristic_instance_id,
    RemoteCharacteristicStopNotificationsCallback callback) {
  characteristic_id_to_notify_session_.erase(characteristic_instance_id);
  std::move(callback).Run();
}
