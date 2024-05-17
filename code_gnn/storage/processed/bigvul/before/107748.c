void FrameLoader::DispatchDocumentElementAvailable() {
  ScriptForbiddenScope forbid_scripts;
  Client()->DocumentElementAvailable();
}
