SVGElement::SVGElement(const QualifiedName& tag_name,
                       Document& document,
                       ConstructionType construction_type)
    : Element(tag_name, &document, construction_type),
      svg_rare_data_(nullptr),
      class_name_(SVGAnimatedString::Create(this, html_names::kClassAttr)) {
  AddToPropertyMap(class_name_);
  SetHasCustomStyleCallbacks();
}
