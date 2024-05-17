void SVGElement::SvgAttributeBaseValChanged(const QualifiedName& attribute) {
  SvgAttributeChanged(attribute);

  if (!HasSVGRareData() || SvgRareData()->WebAnimatedAttributes().IsEmpty())
    return;

  SvgRareData()->SetWebAnimatedAttributesDirty(true);
  GetElementData()->animated_svg_attributes_are_dirty_ = true;
}
