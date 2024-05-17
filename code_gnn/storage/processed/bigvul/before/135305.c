void Document::modifiedStyleSheet(StyleSheet* sheet, StyleResolverUpdateMode updateMode)
{
    if (isActive())
        styleEngine().modifiedStyleSheet(sheet);
    styleResolverChanged(updateMode);
}
