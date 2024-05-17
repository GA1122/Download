void SVGElement::SvgAttributeChanged(const QualifiedName& attr_name) {
  CSSPropertyID prop_id =
      SVGElement::CssPropertyIdForSVGAttributeName(attr_name);
  if (prop_id > 0) {
    InvalidateInstances();
    return;
  }

  if (attr_name == html_names::kClassAttr) {
    ClassAttributeChanged(AtomicString(class_name_->CurrentValue()->Value()));
    InvalidateInstances();
    return;
  }
}
