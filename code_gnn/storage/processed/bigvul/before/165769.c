void SVGElement::AttributeChanged(const AttributeModificationParams& params) {
  Element::AttributeChanged(params);

  if (params.name == html_names::kIdAttr) {
    RebuildAllIncomingReferences();
    InvalidateInstances();
    return;
  }

  if (params.name == html_names::kStyleAttr)
    return;

  SvgAttributeBaseValChanged(params.name);
}
