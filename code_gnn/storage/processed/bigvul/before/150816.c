void WebBluetoothServiceImpl::RemoteCharacteristicStartNotifications(
    const std::string& characteristic_instance_id,
    blink::mojom::WebBluetoothCharacteristicClientAssociatedPtrInfo client,
    RemoteCharacteristicStartNotificationsCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  auto iter =
      characteristic_id_to_notify_session_.find(characteristic_instance_id);
  if (iter != characteristic_id_to_notify_session_.end() &&
      iter->second->gatt_notify_session->IsActive()) {
    std::move(callback).Run(blink::mojom::WebBluetoothResult::SUCCESS);
    return;
  }

  const CacheQueryResult query_result =
      QueryCacheForCharacteristic(characteristic_instance_id);

  if (query_result.outcome == CacheQueryOutcome::BAD_RENDERER) {
    return;
  }

  if (query_result.outcome != CacheQueryOutcome::SUCCESS) {
    RecordStartNotificationsOutcome(query_result.outcome);
    std::move(callback).Run(query_result.GetWebResult());
    return;
  }

  device::BluetoothRemoteGattCharacteristic::Properties notify_or_indicate =
      query_result.characteristic->GetProperties() &
      (device::BluetoothRemoteGattCharacteristic::PROPERTY_NOTIFY |
       device::BluetoothRemoteGattCharacteristic::PROPERTY_INDICATE);
  if (!notify_or_indicate) {
    std::move(callback).Run(
        blink::mojom::WebBluetoothResult::GATT_NOT_SUPPORTED);
    return;
  }

  blink::mojom::WebBluetoothCharacteristicClientAssociatedPtr
      characteristic_client;
  characteristic_client.Bind(std::move(client));

  auto copyable_callback = base::AdaptCallbackForRepeating(std::move(callback));
  query_result.characteristic->StartNotifySession(
      base::Bind(&WebBluetoothServiceImpl::OnStartNotifySessionSuccess,
                 weak_ptr_factory_.GetWeakPtr(),
                 base::Passed(&characteristic_client), copyable_callback),
      base::Bind(&WebBluetoothServiceImpl::OnStartNotifySessionFailed,
                 weak_ptr_factory_.GetWeakPtr(), copyable_callback));
}
