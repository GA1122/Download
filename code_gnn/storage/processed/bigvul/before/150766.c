void WebBluetoothServiceImpl::AdapterPoweredChanged(
    device::BluetoothAdapter* adapter,
    bool powered) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (device_chooser_controller_.get()) {
    device_chooser_controller_->AdapterPoweredChanged(powered);
  }
}
