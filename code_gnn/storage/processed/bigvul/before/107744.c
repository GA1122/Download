void FrameLoader::DidAccessInitialDocument() {
  if (frame_->IsMainFrame()) {
    ScriptForbiddenScope forbid_scripts;
    if (Client())
      Client()->DidAccessInitialDocument();
  }
}
