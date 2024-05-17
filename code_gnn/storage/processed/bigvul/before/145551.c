ConstructHandshakeMessage(base::StringPiece handshake_key,
                          base::span<const uint8_t, 16> client_random_nonce) {
  cbor::CBORValue::MapValue map;
  map.emplace(0, kCableClientHelloMessage);
  map.emplace(1, client_random_nonce);
  auto client_hello = cbor::CBORWriter::Write(cbor::CBORValue(std::move(map)));
  DCHECK(client_hello);

  crypto::HMAC hmac(crypto::HMAC::SHA256);
  if (!hmac.Init(handshake_key))
    return base::nullopt;

  std::array<uint8_t, 32> client_hello_mac;
  if (!hmac.Sign(fido_parsing_utils::ConvertToStringPiece(*client_hello),
                 client_hello_mac.data(), client_hello_mac.size())) {
    return base::nullopt;
  }

  DCHECK_EQ(kClientHelloMessageSize,
            client_hello->size() + kCableHandshakeMacMessageSize);
  std::array<uint8_t, kClientHelloMessageSize> handshake_message;
  std::copy(client_hello->begin(), client_hello->end(),
            handshake_message.begin());
  std::copy(client_hello_mac.begin(),
            client_hello_mac.begin() + kCableHandshakeMacMessageSize,
            handshake_message.begin() + client_hello->size());

  return handshake_message;
}
