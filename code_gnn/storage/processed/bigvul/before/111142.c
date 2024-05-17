RenderLayer* WebPagePrivate::enclosingPositionedAncestorOrSelfIfPositioned(RenderLayer* layer)
{
    return findAncestorOrSelfNotMatching(&isPositionedContainer, layer);
}
