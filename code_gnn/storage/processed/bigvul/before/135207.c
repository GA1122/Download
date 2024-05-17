void Document::didRemoveAllPendingStylesheet()
{
    styleResolverMayHaveChanged();

    if (HTMLImportLoader* import = importLoader())
        import->didRemoveAllPendingStylesheet();
    if (!haveImportsLoaded())
        return;
    didLoadAllScriptBlockingResources();
}
