void Document::UpdateUseShadowTreesIfNeeded() {
  ScriptForbiddenScope forbid_script;

  if (use_elements_needing_update_.IsEmpty())
    return;

  HeapHashSet<Member<SVGUseElement>> elements;
  use_elements_needing_update_.swap(elements);
  for (SVGUseElement* element : elements)
    element->BuildPendingResource();
}
