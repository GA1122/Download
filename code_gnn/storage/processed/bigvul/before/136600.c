std::unique_ptr<SourceLocation> DocumentLoader::CopySourceLocation() const {
  return source_location_ ? source_location_->Clone() : nullptr;
}
