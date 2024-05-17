void HTMLCanvasElement::ParseAttribute(
    const AttributeModificationParams& params) {
  if (params.name == kWidthAttr || params.name == kHeightAttr)
    Reset();
  HTMLElement::ParseAttribute(params);
}
