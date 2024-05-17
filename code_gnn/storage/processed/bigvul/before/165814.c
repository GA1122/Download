void SVGElement::RebuildAllIncomingReferences() {
  if (!HasSVGRareData())
    return;

  const SVGElementSet& incoming_references =
      SvgRareData()->IncomingReferences();

  HeapVector<Member<SVGElement>> incoming_references_snapshot;
  CopyToVector(incoming_references, incoming_references_snapshot);

  for (SVGElement* source_element : incoming_references_snapshot) {
    if (incoming_references.Contains(source_element))
      source_element->SvgAttributeChanged(svg_names::kHrefAttr);
  }
}
