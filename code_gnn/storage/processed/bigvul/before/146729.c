void Document::DidLoadAllImports() {
  if (!HaveScriptBlockingStylesheetsLoaded())
    return;
  if (!ImportLoader())
    StyleResolverMayHaveChanged();
  DidLoadAllScriptBlockingResources();
}
