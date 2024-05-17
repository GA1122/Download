std::vector<uint8_t> GetTestAttestationObjectBytes() {
  auto test_authenticator_object =
      fido_parsing_utils::Materialize(kFormatFidoU2fCBOR);
  fido_parsing_utils::Append(&test_authenticator_object, kAttStmtCBOR);
  fido_parsing_utils::Append(&test_authenticator_object,
                             test_data::kU2fAttestationStatementCBOR);
  fido_parsing_utils::Append(&test_authenticator_object, kAuthDataCBOR);
  auto test_authenticator_data = GetTestAuthenticatorDataBytes();
  fido_parsing_utils::Append(&test_authenticator_object,
                             test_authenticator_data);
  return test_authenticator_object;
}
