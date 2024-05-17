bool CanMouseDownStartSelect(Node* node) {
  if (!node || !node->GetLayoutObject())
    return true;

  if (!node->CanStartSelection())
    return false;

  return true;
}
