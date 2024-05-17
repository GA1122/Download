bool Document::hidden() const {
  return GetPageVisibilityState() != mojom::PageVisibilityState::kVisible;
}
