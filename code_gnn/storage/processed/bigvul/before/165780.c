scoped_refptr<ComputedStyle> SVGElement::CustomStyleForLayoutObject() {
  if (!CorrespondingElement())
    return GetDocument().EnsureStyleResolver().StyleForElement(this);

  const ComputedStyle* style = nullptr;
  if (Element* parent = ParentOrShadowHostElement())
    style = parent->GetComputedStyle();

  return GetDocument().EnsureStyleResolver().StyleForElement(
      CorrespondingElement(), style, style);
}
