std::vector<uint8_t> GetTestCredentialRawIdBytes() {
  return fido_parsing_utils::Materialize(test_data::kU2fSignKeyHandle);
}
