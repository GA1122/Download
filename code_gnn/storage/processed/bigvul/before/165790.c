SVGResourceClient* SVGElement::GetSVGResourceClient() {
  if (!HasSVGRareData())
    return nullptr;
  return SvgRareData()->GetSVGResourceClient();
}
