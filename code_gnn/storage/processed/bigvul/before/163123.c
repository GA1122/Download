bool DoesBuilderHaveFutureData(size_t index) {
  return index < kTotalRawBlobs && (index % 2 != 0 || index % 3 == 0);
}
