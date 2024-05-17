void notifyFormRemovedFromTree(const T& elements, Node& root) {
  for (const auto& element : elements)
    element->formRemovedFromTree(root);
}
