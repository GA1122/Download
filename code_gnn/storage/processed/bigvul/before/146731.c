void Document::DidRemoveAllPendingStylesheet() {
  StyleResolverMayHaveChanged();

  if (HTMLImportLoader* import = ImportLoader())
    import->DidRemoveAllPendingStylesheet();
  if (!HaveImportsLoaded())
    return;
  DidLoadAllScriptBlockingResources();
}
