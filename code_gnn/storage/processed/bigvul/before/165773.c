void SVGElement::ClearWebAnimatedAttributes() {
  if (!HasSVGRareData())
    return;
  for (const QualifiedName* attribute :
       SvgRareData()->WebAnimatedAttributes()) {
    ForSelfAndInstances(this, [&attribute](SVGElement* element) {
      if (SVGAnimatedPropertyBase* animated_property =
              element->PropertyFromAttribute(*attribute)) {
        animated_property->AnimationEnded();
        NotifyAnimValChanged(element, *attribute);
      }
    });
  }
  SvgRareData()->WebAnimatedAttributes().clear();
}
