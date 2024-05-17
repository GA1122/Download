ReadCTAPMakeCredentialResponse(base::span<const uint8_t> buffer) {
  if (buffer.size() <= kResponseCodeLength)
    return base::nullopt;

  base::Optional<CBOR> decoded_response =
      cbor::CBORReader::Read(buffer.subspan(1));
  if (!decoded_response || !decoded_response->is_map())
    return base::nullopt;

  const auto& decoded_map = decoded_response->GetMap();
  auto it = decoded_map.find(CBOR(1));
  if (it == decoded_map.end() || !it->second.is_string())
    return base::nullopt;
  auto format = it->second.GetString();

  it = decoded_map.find(CBOR(2));
  if (it == decoded_map.end() || !it->second.is_bytestring())
    return base::nullopt;

  auto authenticator_data =
      AuthenticatorData::DecodeAuthenticatorData(it->second.GetBytestring());
  if (!authenticator_data)
    return base::nullopt;

  it = decoded_map.find(CBOR(3));
  if (it == decoded_map.end() || !it->second.is_map())
    return base::nullopt;

  return AuthenticatorMakeCredentialResponse(
      AttestationObject(std::move(*authenticator_data),
                        std::make_unique<OpaqueAttestationStatement>(
                            format, it->second.Clone())));
}
