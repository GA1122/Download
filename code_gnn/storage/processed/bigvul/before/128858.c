Element* SVGDocumentExtensions::removeElementFromPendingResourcesForRemoval(const AtomicString& id)
{
    if (id.isEmpty())
        return 0;

    SVGPendingElements* resourceSet = m_pendingResourcesForRemoval.get(id);
    if (!resourceSet || resourceSet->isEmpty())
        return 0;

    SVGPendingElements::iterator firstElement = resourceSet->begin();
    Element* element = *firstElement;

    resourceSet->remove(firstElement);

    if (resourceSet->isEmpty())
        removePendingResourceForRemoval(id);

    return element;
}
