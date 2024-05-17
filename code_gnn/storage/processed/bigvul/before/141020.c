void Document::DidLoadAllImports() {
  if (!HaveScriptBlockingStylesheetsLoaded())
    return;
  DidLoadAllScriptBlockingResources();
}
