void SVGElement::DetachLayoutTree(const AttachContext& context) {
  Element::DetachLayoutTree(context);
  if (SVGElement* element = CorrespondingElement())
    element->RemoveInstanceMapping(this);
  if (HasSVGRareData())
    SvgRareData()->ClearOverriddenComputedStyle();
}
