void FrameLoader::FinishedParsing() {
  if (state_machine_.CreatingInitialEmptyDocument())
    return;

  progress_tracker_->FinishedParsing();

  if (Client()) {
    ScriptForbiddenScope forbid_scripts;
    Client()->DispatchDidFinishDocumentLoad();
  }

  if (Client()) {
    Client()->RunScriptsAtDocumentReady(
        document_loader_ ? document_loader_->IsCommittedButEmpty() : true);
  }

  if (frame_->View()) {
    frame_->View()->RestoreScrollbar();

    ProcessFragment(frame_->GetDocument()->Url(), document_loader_->LoadType(),
                    kNavigationToDifferentDocument);
  }

  frame_->GetDocument()->CheckCompleted();
}
