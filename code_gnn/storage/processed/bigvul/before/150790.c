void WebBluetoothServiceImpl::NotifyCharacteristicValueChanged(
    const std::string& characteristic_instance_id,
    const std::vector<uint8_t>& value) {
  auto iter =
      characteristic_id_to_notify_session_.find(characteristic_instance_id);
  if (iter != characteristic_id_to_notify_session_.end()) {
    iter->second->characteristic_client->RemoteCharacteristicValueChanged(
        value);
   }
 }
