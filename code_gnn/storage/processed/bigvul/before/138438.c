void Document::processHttpEquivDefaultStyle(const AtomicString& content)
{
    styleEngine().setSelectedStylesheetSetName(content);
    styleEngine().setPreferredStylesheetSetName(content);
    styleResolverChanged();
}
