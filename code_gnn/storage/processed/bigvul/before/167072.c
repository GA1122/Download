int BluetoothSocketAsyncApiFunction::AddSocket(BluetoothApiSocket* socket) {
  return manager_->Add(socket);
}
