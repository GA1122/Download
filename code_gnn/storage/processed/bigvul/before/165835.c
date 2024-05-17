void SVGElement::Trace(blink::Visitor* visitor) {
  visitor->Trace(elements_with_relative_lengths_);
  visitor->Trace(attribute_to_property_map_);
  visitor->Trace(svg_rare_data_);
  visitor->Trace(class_name_);
  Element::Trace(visitor);
}
