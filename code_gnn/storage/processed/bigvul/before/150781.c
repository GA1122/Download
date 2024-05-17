device::BluetoothAdapter* WebBluetoothServiceImpl::GetAdapter() {
  return BluetoothAdapterFactoryWrapper::Get().GetAdapter(this);
}
