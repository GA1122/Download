void HTMLFormControlElement::fieldSetAncestorsSetNeedsValidityCheck(
    Node* node) {
  if (!node)
    return;
  for (HTMLFieldSetElement* fieldSet =
           Traversal<HTMLFieldSetElement>::firstAncestorOrSelf(*node);
       fieldSet;
       fieldSet = Traversal<HTMLFieldSetElement>::firstAncestor(*fieldSet)) {
    fieldSet->pseudoStateChanged(CSSSelector::PseudoValid);
    fieldSet->pseudoStateChanged(CSSSelector::PseudoInvalid);
  }
}
