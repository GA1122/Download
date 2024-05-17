TreeScope& SVGElement::TreeScopeForIdResolution() const {
  const SVGElement* tree_scope_element = this;
  if (const SVGElement* element = CorrespondingUseElement())
    tree_scope_element = element;
  return tree_scope_element->GetTreeScope();
}
