void Document::addToTopLayer(Element* element)
{
    if (element->isInTopLayer())
        return;
    ASSERT(!m_topLayerElements.contains(element));
    m_topLayerElements.append(element);
    element->setIsInTopLayer(true);
}
