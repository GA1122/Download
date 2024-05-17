static HTMLInputElement* AsFileInput(Node* node) {
  DCHECK(node);
  for (; node; node = node->OwnerShadowHost()) {
    if (IsHTMLInputElement(*node) &&
        ToHTMLInputElement(node)->type() == InputTypeNames::file)
      return ToHTMLInputElement(node);
  }
  return nullptr;
}
