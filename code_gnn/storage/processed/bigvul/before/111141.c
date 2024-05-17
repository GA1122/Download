RenderLayer* WebPagePrivate::enclosingFixedPositionedAncestorOrSelfIfFixedPositioned(RenderLayer* layer)
{
    return findAncestorOrSelfNotMatching(&isFixedPositionedContainer, layer);
}
