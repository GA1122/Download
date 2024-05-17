void Document::didLoadAllImports()
{
    if (!haveStylesheetsLoaded())
        return;
    if (!importLoader())
        styleResolverMayHaveChanged();
    didLoadAllScriptBlockingResources();
}
