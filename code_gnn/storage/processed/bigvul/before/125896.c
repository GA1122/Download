void CompareQuicDataWithHexError(
    const string& description,
    QuicData* actual,
    QuicData* expected) {
  CompareCharArraysWithHexError(
      description,
      actual->data(), actual->length(),
      expected->data(), expected->length());
}
