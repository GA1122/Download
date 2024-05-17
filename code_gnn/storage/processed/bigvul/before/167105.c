void BluetoothSocketDisconnectFunction::OnSuccess() {
  DCHECK_CURRENTLY_ON(work_thread_id());
  Respond(ArgumentList(bluetooth_socket::Disconnect::Results::Create()));
}
