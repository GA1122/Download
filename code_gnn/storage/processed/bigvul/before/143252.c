void Document::scheduleSVGFilterLayerUpdateHack(Element& element)
{
    if (element.getStyleChangeType() == NeedsReattachStyleChange)
        return;
    element.setSVGFilterNeedsLayerUpdate();
    m_layerUpdateSVGFilterElements.add(&element);
    scheduleLayoutTreeUpdateIfNeeded();
}
