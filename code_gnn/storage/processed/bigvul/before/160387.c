size_t LargeObjectPage::objectPayloadSizeForTesting() {
  markAsSwept();
  return payloadSize();
}
