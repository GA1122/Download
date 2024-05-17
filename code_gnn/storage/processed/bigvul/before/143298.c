void Document::updateUseShadowTreesIfNeeded()
{
    ScriptForbiddenScope forbidScript;

    if (m_useElementsNeedingUpdate.isEmpty())
        return;

    HeapHashSet<Member<SVGUseElement>> elements;
    m_useElementsNeedingUpdate.swap(elements);
    for (SVGUseElement* element : elements)
        element->buildPendingResource();
}
