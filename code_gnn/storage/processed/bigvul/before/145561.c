std::vector<uint8_t> GetTestSignResponse() {
  return fido_parsing_utils::Materialize(test_data::kTestU2fSignResponse);
}
