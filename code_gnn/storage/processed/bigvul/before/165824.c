void SVGElement::SetAnimatedAttribute(const QualifiedName& attribute,
                                      SVGPropertyBase* value) {
  ForSelfAndInstances(this, [&attribute, &value](SVGElement* element) {
    if (SVGAnimatedPropertyBase* animated_property =
            element->PropertyFromAttribute(attribute))
      animated_property->SetAnimatedValue(value);
  });
}
