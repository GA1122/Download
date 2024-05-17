void Document::removedStyleSheet(StyleSheet* sheet, StyleResolverUpdateMode updateMode)
{
    if (isActive())
        styleEngine().modifiedStyleSheet(sheet);
    styleResolverChanged(updateMode);
}
