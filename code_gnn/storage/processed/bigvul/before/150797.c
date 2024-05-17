void WebBluetoothServiceImpl::OnCreateGATTConnectionSuccess(
    const blink::WebBluetoothDeviceId& device_id,
    base::TimeTicks start_time,
    blink::mojom::WebBluetoothServerClientAssociatedPtr client,
    RemoteServerConnectCallback callback,
    std::unique_ptr<device::BluetoothGattConnection> connection) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  RecordConnectGATTTimeSuccess(base::TimeTicks::Now() - start_time);
  RecordConnectGATTOutcome(UMAConnectGATTOutcome::SUCCESS);

  if (connected_devices_->IsConnectedToDeviceWithId(device_id)) {
    DVLOG(1) << "Already connected.";
    std::move(callback).Run(blink::mojom::WebBluetoothResult::SUCCESS);
    return;
  }

  std::move(callback).Run(blink::mojom::WebBluetoothResult::SUCCESS);
  connected_devices_->Insert(device_id, std::move(connection),
                             std::move(client));
}
