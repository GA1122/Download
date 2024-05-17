void WebBluetoothServiceImpl::OnCreateGATTConnectionFailed(
    base::TimeTicks start_time,
    RemoteServerConnectCallback callback,
    device::BluetoothDevice::ConnectErrorCode error_code) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  RecordConnectGATTTimeFailed(base::TimeTicks::Now() - start_time);
  std::move(callback).Run(TranslateConnectErrorAndRecord(error_code));
}
