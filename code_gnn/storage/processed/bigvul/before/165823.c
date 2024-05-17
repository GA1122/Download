void SVGElement::SendSVGLoadEventToSelfAndAncestorChainIfPossible() {
  if (IsOutermostSVGSVGElement())
    return;

  Element* parent = ParentOrShadowHostElement();
  if (!SendSVGLoadEventIfPossible())
    return;

  if (GetDocument().LoadEventFinished())
    return;

  if (!parent || !parent->IsSVGElement())
    return;

  ToSVGElement(parent)->SendSVGLoadEventToSelfAndAncestorChainIfPossible();
}
