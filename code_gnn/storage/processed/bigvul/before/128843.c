void SVGDocumentExtensions::clearHasPendingResourcesIfPossible(Element* element)
{
    if (!isElementPendingResources(element))
        element->clearHasPendingResources();
}
