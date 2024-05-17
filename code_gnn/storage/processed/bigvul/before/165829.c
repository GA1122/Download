void SVGElement::SetUseOverrideComputedStyle(bool value) {
  if (HasSVGRareData())
    SvgRareData()->SetUseOverrideComputedStyle(value);
}
