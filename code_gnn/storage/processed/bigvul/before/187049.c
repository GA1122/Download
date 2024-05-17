  bool SVGElement::HasSVGParent() const {
  return ParentOrShadowHostElement() &&
         ParentOrShadowHostElement()->IsSVGElement();
//   Element* parent = FlatTreeTraversal::ParentElement(*this);
//   return parent && parent->IsSVGElement();
  }