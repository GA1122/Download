SVGUseElement* SVGElement::CorrespondingUseElement() const {
  if (ShadowRoot* root = ContainingShadowRoot()) {
    if (IsSVGUseElement(root->host()))
      return &ToSVGUseElement(root->host());
  }
  return nullptr;
}
