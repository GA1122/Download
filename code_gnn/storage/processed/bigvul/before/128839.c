void SVGDocumentExtensions::addPendingResource(const AtomicString& id, Element* element)
{
    ASSERT(element);
    ASSERT(element->inDocument());

    if (id.isEmpty())
        return;

    HashMap<AtomicString, OwnPtr<SVGPendingElements> >::AddResult result = m_pendingResources.add(id, nullptr);
    if (result.isNewEntry)
        result.storedValue->value = adoptPtr(new SVGPendingElements);
    result.storedValue->value->add(element);

    element->setHasPendingResources();
}
