void Document::removeFromTopLayer(Element* element)
{
    if (!element->isInTopLayer())
        return;
    size_t position = m_topLayerElements.find(element);
    ASSERT(position != kNotFound);
    m_topLayerElements.remove(position);
    element->setIsInTopLayer(false);
}
