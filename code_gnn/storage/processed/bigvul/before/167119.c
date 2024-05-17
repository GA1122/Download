ExtensionFunction::ResponseAction BluetoothSocketGetSocketsFunction::Run() {
  std::vector<bluetooth_socket::SocketInfo> socket_infos;
  base::hash_set<int>* resource_ids = GetSocketIds();
  if (resource_ids) {
    for (int socket_id : *resource_ids) {
      BluetoothApiSocket* socket = GetSocket(socket_id);
      if (socket) {
        socket_infos.push_back(CreateSocketInfo(socket_id, socket));
      }
    }
  }
  return RespondNow(ArgumentList(
      bluetooth_socket::GetSockets::Results::Create(socket_infos)));
}
