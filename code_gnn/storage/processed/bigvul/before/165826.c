void SVGElement::SetInstanceUpdatesBlocked(bool value) {
  if (HasSVGRareData())
    SvgRareData()->SetInstanceUpdatesBlocked(value);
}
