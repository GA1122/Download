  std::unique_ptr<SourceLocation> TakeLocation() {
    return std::move(location_);
  }
