base::Optional<AuthenticatorGetAssertionResponse> ReadCTAPGetAssertionResponse(
    base::span<const uint8_t> buffer) {
  if (buffer.size() <= kResponseCodeLength)
    return base::nullopt;

  base::Optional<CBOR> decoded_response =
      cbor::CBORReader::Read(buffer.subspan(1));

  if (!decoded_response || !decoded_response->is_map())
    return base::nullopt;

  auto& response_map = decoded_response->GetMap();

  auto it = response_map.find(CBOR(2));
  if (it == response_map.end() || !it->second.is_bytestring())
    return base::nullopt;

  auto auth_data =
      AuthenticatorData::DecodeAuthenticatorData(it->second.GetBytestring());
  if (!auth_data)
    return base::nullopt;

  it = response_map.find(CBOR(3));
  if (it == response_map.end() || !it->second.is_bytestring())
    return base::nullopt;

  auto signature = it->second.GetBytestring();
  AuthenticatorGetAssertionResponse response(std::move(*auth_data),
                                             std::move(signature));

  it = response_map.find(CBOR(1));
  if (it != response_map.end()) {
    auto credential =
        PublicKeyCredentialDescriptor::CreateFromCBORValue(it->second);
    if (!credential)
      return base::nullopt;
    response.SetCredential(std::move(*credential));
  }

  it = response_map.find(CBOR(4));
  if (it != response_map.end()) {
    auto user = PublicKeyCredentialUserEntity::CreateFromCBORValue(it->second);
    if (!user)
      return base::nullopt;
    response.SetUserEntity(std::move(*user));
  }

  it = response_map.find(CBOR(5));
  if (it != response_map.end()) {
    if (!it->second.is_unsigned())
      return base::nullopt;

    response.SetNumCredentials(it->second.GetUnsigned());
  }

  return base::Optional<AuthenticatorGetAssertionResponse>(std::move(response));
}
