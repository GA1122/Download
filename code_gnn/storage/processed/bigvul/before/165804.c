bool SVGElement::IsOutermostSVGSVGElement() const {
  if (!IsSVGSVGElement(*this))
    return false;

  if (!parentNode())
    return true;

  if (IsSVGForeignObjectElement(*parentNode()))
    return true;

  if (InUseShadowTree() && ParentOrShadowHostElement() &&
      ParentOrShadowHostElement()->IsSVGElement())
    return false;

  return !parentNode()->IsSVGElement();
}
