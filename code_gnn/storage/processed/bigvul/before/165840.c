SVGSVGElement* SVGElement::ownerSVGElement() const {
  ContainerNode* n = ParentOrShadowHostNode();
  while (n) {
    if (IsSVGSVGElement(*n))
      return ToSVGSVGElement(n);

    n = n->ParentOrShadowHostNode();
  }

  return nullptr;
}
