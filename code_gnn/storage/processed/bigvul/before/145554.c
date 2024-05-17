std::string FidoCableHandshakeHandler::GetEncryptionKeyAfterSuccessfulHandshake(
    base::span<const uint8_t, 16> authenticator_random_nonce) const {
  std::vector<uint8_t> nonce_message;
  fido_parsing_utils::Append(&nonce_message, nonce_);
  fido_parsing_utils::Append(&nonce_message, client_session_random_);
  fido_parsing_utils::Append(&nonce_message, authenticator_random_nonce);
  return GenerateKey(
      fido_parsing_utils::ConvertToStringPiece(session_pre_key_),
      fido_parsing_utils::ConvertToStringPiece(
          fido_parsing_utils::CreateSHA256Hash(
              fido_parsing_utils::ConvertToStringPiece(nonce_message))),
      kCableDeviceEncryptionKeyInfo);
}
