std::vector<uint8_t> GetTestCorruptedSignResponse(size_t length) {
  DCHECK_LE(length, arraysize(test_data::kTestU2fSignResponse));
  return fido_parsing_utils::Materialize(fido_parsing_utils::ExtractSpan(
      test_data::kTestU2fSignResponse, 0, length));
}
