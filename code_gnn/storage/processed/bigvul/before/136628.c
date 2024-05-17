void DocumentLoader::SetSourceLocation(
    std::unique_ptr<SourceLocation> source_location) {
  source_location_ = std::move(source_location);
}
