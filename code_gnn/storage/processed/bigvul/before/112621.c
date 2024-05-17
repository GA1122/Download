PassRefPtr<DOMNamedFlowCollection> Document::webkitGetNamedFlows()
{
    if (!cssRegionsEnabled() || !renderer())
        return 0;

    updateStyleIfNeeded();

    return namedFlows()->createCSSOMSnapshot();
}
