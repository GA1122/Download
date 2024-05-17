bool SVGElement::InstanceUpdatesBlocked() const {
  return HasSVGRareData() && SvgRareData()->InstanceUpdatesBlocked();
}
