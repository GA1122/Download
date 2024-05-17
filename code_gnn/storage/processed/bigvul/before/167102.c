void BluetoothSocketSendFunction::OnError(
    BluetoothApiSocket::ErrorReason reason,
    const std::string& message) {
  DCHECK_CURRENTLY_ON(work_thread_id());
  Respond(Error(message));
}
