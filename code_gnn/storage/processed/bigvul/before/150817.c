void WebBluetoothServiceImpl::RemoteCharacteristicStopNotifications(
    const std::string& characteristic_instance_id,
    RemoteCharacteristicStopNotificationsCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  const CacheQueryResult query_result =
      QueryCacheForCharacteristic(characteristic_instance_id);

  if (query_result.outcome == CacheQueryOutcome::BAD_RENDERER) {
    return;
  }

  auto notify_session_iter =
      characteristic_id_to_notify_session_.find(characteristic_instance_id);
  if (notify_session_iter == characteristic_id_to_notify_session_.end()) {
    std::move(callback).Run();
    return;
  }
  notify_session_iter->second->gatt_notify_session->Stop(
      base::Bind(&WebBluetoothServiceImpl::OnStopNotifySessionComplete,
                 weak_ptr_factory_.GetWeakPtr(), characteristic_instance_id,
                 base::Passed(&callback)));
}
