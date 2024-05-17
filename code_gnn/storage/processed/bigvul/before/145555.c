void FidoCableHandshakeHandler::InitiateCableHandshake(
    FidoDevice::DeviceCallback callback) {
  auto handshake_message =
      ConstructHandshakeMessage(handshake_key_, client_session_random_);
  if (!handshake_message) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE, base::BindOnce(std::move(callback), base::nullopt));
    return;
  }

  cable_device_->SendHandshakeMessage(
      fido_parsing_utils::Materialize(*handshake_message), std::move(callback));
}
