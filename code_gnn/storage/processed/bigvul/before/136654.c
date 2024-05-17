void FrameLoader::DidAccessInitialDocument() {
  if (IsLoadingMainFrame()) {
    ScriptForbiddenScope forbid_scripts;
    if (Client())
      Client()->DidAccessInitialDocument();
  }
}
