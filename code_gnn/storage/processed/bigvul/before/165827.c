void SVGElement::SetNeedsStyleRecalcForInstances(
    StyleChangeType change_type,
    const StyleChangeReasonForTracing& reason) {
  const HeapHashSet<WeakMember<SVGElement>>& set = InstancesForElement();
  if (set.IsEmpty())
    return;

  for (SVGElement* instance : set)
    instance->SetNeedsStyleRecalc(change_type, reason);
}
