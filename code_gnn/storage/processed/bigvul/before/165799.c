const HeapHashSet<WeakMember<SVGElement>>& SVGElement::InstancesForElement()
    const {
  if (!HasSVGRareData())
    return EmptyInstances();
  return SvgRareData()->ElementInstances();
}
