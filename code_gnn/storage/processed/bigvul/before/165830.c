void SVGElement::SetWebAnimatedAttribute(const QualifiedName& attribute,
                                         SVGPropertyBase* value) {
  ForSelfAndInstances(this, [&attribute, &value](SVGElement* element) {
    if (SVGAnimatedPropertyBase* animated_property =
            element->PropertyFromAttribute(attribute)) {
      animated_property->SetAnimatedValue(value);
      NotifyAnimValChanged(element, attribute);
    }
  });
  EnsureSVGRareData()->WebAnimatedAttributes().insert(&attribute);
}
