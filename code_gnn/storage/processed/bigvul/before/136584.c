DisplayItemClient& TestPaintArtifact::NewClient() {
  dummy_clients_.push_back(std::make_unique<DummyRectClient>());
  return *dummy_clients_.back();
}
