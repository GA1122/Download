void SVGElement::RemoveInstanceMapping(SVGElement* instance) {
  DCHECK(instance);
  DCHECK(instance->InUseShadowTree());

  if (!HasSVGRareData())
    return;

  HeapHashSet<WeakMember<SVGElement>>& instances =
      SvgRareData()->ElementInstances();

  instances.erase(instance);
}
