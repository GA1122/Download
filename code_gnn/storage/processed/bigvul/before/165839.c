void SVGElement::WillRecalcStyle(StyleRecalcChange change) {
  if (!HasSVGRareData())
    return;
  if (change > kNoChange || NeedsStyleRecalc())
    SvgRareData()->SetNeedsOverrideComputedStyleUpdate();
}
