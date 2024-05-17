const ComputedStyle* SVGElement::EnsureComputedStyle(
    PseudoId pseudo_element_specifier) {
  if (!HasSVGRareData() || !SvgRareData()->UseOverrideComputedStyle())
    return Element::EnsureComputedStyle(pseudo_element_specifier);

  const ComputedStyle* parent_style = nullptr;
  if (Element* parent = ParentOrShadowHostElement()) {
    if (LayoutObject* layout_object = parent->GetLayoutObject())
      parent_style = layout_object->Style();
  }

  return SvgRareData()->OverrideComputedStyle(this, parent_style);
}
