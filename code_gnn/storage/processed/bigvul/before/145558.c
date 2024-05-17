std::vector<uint8_t> GetTestAuthenticatorDataBytes() {
  auto test_authenticator_data =
      fido_parsing_utils::Materialize(kTestAuthenticatorDataPrefix);
  auto test_attested_data = GetTestAttestedCredentialDataBytes();
  fido_parsing_utils::Append(&test_authenticator_data, test_attested_data);
  return test_authenticator_data;
}
