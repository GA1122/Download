BluetoothSocketAbstractConnectFunction::Run() {
  DCHECK_CURRENTLY_ON(work_thread_id());
  device::BluetoothAdapterFactory::GetAdapter(
      base::Bind(&BluetoothSocketAbstractConnectFunction::OnGetAdapter, this));
  return did_respond() ? AlreadyResponded() : RespondLater();
}
