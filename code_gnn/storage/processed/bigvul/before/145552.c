FidoCableHandshakeHandler::FidoCableHandshakeHandler(
    FidoCableDevice* cable_device,
    base::span<const uint8_t, 8> nonce,
    base::span<const uint8_t, 32> session_pre_key)
    : cable_device_(cable_device),
      nonce_(fido_parsing_utils::Materialize(nonce)),
      session_pre_key_(fido_parsing_utils::Materialize(session_pre_key)),
      handshake_key_(GenerateKey(
          fido_parsing_utils::ConvertToStringPiece(session_pre_key_),
          fido_parsing_utils::ConvertToStringPiece(nonce_),
          kCableHandshakeKeyInfo)),
      weak_factory_(this) {
  crypto::RandBytes(client_session_random_.data(),
                    client_session_random_.size());
}
