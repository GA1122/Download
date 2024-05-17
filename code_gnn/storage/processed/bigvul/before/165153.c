void NotifyFormRemovedFromTree(const T& elements, Node& root) {
  for (const auto& element : elements)
    element->FormRemovedFromTree(root);
}
