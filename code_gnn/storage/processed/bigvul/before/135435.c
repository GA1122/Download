void Document::updateUseShadowTreesIfNeeded()
{
    ScriptForbiddenScope forbidScript;

    if (m_useElementsNeedingUpdate.isEmpty())
        return;

    WillBeHeapVector<RawPtrWillBeMember<SVGUseElement>> elements;
    copyToVector(m_useElementsNeedingUpdate, elements);
    m_useElementsNeedingUpdate.clear();

    for (SVGUseElement* element : elements)
        element->buildPendingResource();
}
