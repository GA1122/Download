SVGElement* SVGElement::CorrespondingElement() const {
  DCHECK(!HasSVGRareData() || !SvgRareData()->CorrespondingElement() ||
         ContainingShadowRoot());
  return HasSVGRareData() ? SvgRareData()->CorrespondingElement() : nullptr;
}
