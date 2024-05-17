SVGElement* SVGElement::viewportElement() const {
  ContainerNode* n = ParentOrShadowHostNode();
  while (n) {
    if (IsSVGSVGElement(*n) || IsSVGImageElement(*n) || IsSVGSymbolElement(*n))
      return ToSVGElement(n);

    n = n->ParentOrShadowHostNode();
  }

  return nullptr;
}
