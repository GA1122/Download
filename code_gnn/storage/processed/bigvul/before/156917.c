void DocumentLoader::SetSourceLocation(
    const WebSourceLocation& source_location) {
  std::unique_ptr<SourceLocation> location =
      SourceLocation::Create(source_location.url, source_location.line_number,
                             source_location.column_number, nullptr);
  source_location_ = std::move(location);
}
