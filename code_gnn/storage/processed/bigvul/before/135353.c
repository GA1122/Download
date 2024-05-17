void Document::scheduleSVGFilterLayerUpdateHack(Element& element)
{
    if (element.styleChangeType() == NeedsReattachStyleChange)
        return;
    element.setSVGFilterNeedsLayerUpdate();
    m_layerUpdateSVGFilterElements.add(&element);
    scheduleLayoutTreeUpdateIfNeeded();
}
