void Document::styleResolverMayHaveChanged()
{
    styleResolverChanged(hasNodesWithPlaceholderStyle() ? FullStyleUpdate : AnalyzedStyleUpdate);
}
