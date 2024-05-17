void ManifestUmaUtil::ParseFailed() {
  UMA_HISTOGRAM_BOOLEAN(kUMANameParseSuccess, false);
}
