void SelectionController::ContextDestroyed(Document*) {
  original_base_in_flat_tree_ = PositionInFlatTreeWithAffinity();
}
