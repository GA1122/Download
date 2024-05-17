void SVGElement::AddToPropertyMap(SVGAnimatedPropertyBase* property) {
  attribute_to_property_map_.Set(property->AttributeName(), property);
}
