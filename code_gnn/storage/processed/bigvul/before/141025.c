void Document::DidRemoveAllPendingStylesheet() {
  if (HTMLImportLoader* import = ImportLoader())
    import->DidRemoveAllPendingStylesheet();
  if (!HaveImportsLoaded())
    return;
  DidLoadAllScriptBlockingResources();
}
