void SVGElement::InvalidateAnimatedAttribute(const QualifiedName& attribute) {
  ForSelfAndInstances(this, [&attribute](SVGElement* element) {
    NotifyAnimValChanged(element, attribute);
  });
}
