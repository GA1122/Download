void FrameLoader::CommitProvisionalLoad() {
  DCHECK(Client()->HasWebView());

  if (frame_->GetDocument()) {
    scoped_refptr<const SecurityOrigin> security_origin =
        SecurityOrigin::Create(provisional_document_loader_->Url());
    provisional_document_loader_->GetTiming()
        .SetHasSameOriginAsPreviousDocument(
            security_origin->CanRequest(frame_->GetDocument()->Url()));
  }
  virtual_time_pauser_.UnpauseVirtualTime();

  if (!PrepareForCommit())
    return;

  Client()->TransitionToCommittedForNewPage();

  if (!state_machine_.CommittedFirstRealDocumentLoad() ||
      !DocumentLoader::WillLoadUrlAsEmpty(document_loader_->Url())) {
    frame_->GetNavigationScheduler().Cancel();
  }
}
