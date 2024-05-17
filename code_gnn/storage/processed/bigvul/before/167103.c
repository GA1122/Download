void BluetoothSocketListenFunction::OnGetAdapter(
    scoped_refptr<device::BluetoothAdapter> adapter) {
  DCHECK_CURRENTLY_ON(work_thread_id());
  BluetoothApiSocket* socket = GetSocket(socket_id());
  if (!socket) {
    Respond(Error(kSocketNotFoundError));
    return;
  }

  device::BluetoothUUID bluetooth_uuid(uuid());
  if (!bluetooth_uuid.IsValid()) {
    Respond(Error(kInvalidUuidError));
    return;
  }

  BluetoothPermissionRequest param(uuid());
  if (!BluetoothManifestData::CheckRequest(extension(), param)) {
    Respond(Error(kPermissionDeniedError));
    return;
  }

  std::unique_ptr<std::string> name;
  if (socket->name())
    name.reset(new std::string(*socket->name()));

  CreateService(
      adapter, bluetooth_uuid, std::move(name),
      base::Bind(&BluetoothSocketListenFunction::OnCreateService, this),
      base::Bind(&BluetoothSocketListenFunction::OnCreateServiceError, this));
}
