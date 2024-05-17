void Document::unscheduleSVGFilterLayerUpdateHack(Element& element)
{
    element.clearSVGFilterNeedsLayerUpdate();
    m_layerUpdateSVGFilterElements.remove(&element);
}
