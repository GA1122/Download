PassRefPtr<DOMNamedFlowCollection> Document::webkitGetNamedFlows()
{
    if (!RuntimeEnabledFeatures::cssRegionsEnabled() || !renderer())
        return 0;

    updateStyleIfNeeded();

    return namedFlows()->createCSSOMSnapshot();
}
