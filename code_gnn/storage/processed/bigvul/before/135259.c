bool Document::hasSVGRootNode() const
{
    return isSVGSVGElement(documentElement());
}
