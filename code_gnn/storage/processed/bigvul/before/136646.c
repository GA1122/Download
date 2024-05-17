void FrameLoader::CommitProvisionalLoad() {
  DCHECK(Client()->HasWebView());

  if (frame_->GetDocument()) {
    scoped_refptr<const SecurityOrigin> security_origin =
        SecurityOrigin::Create(provisional_document_loader_->Url());
    provisional_document_loader_->GetTiming()
        .SetHasSameOriginAsPreviousDocument(
            security_origin->CanRequest(frame_->GetDocument()->Url()));
  }

  if (!PrepareForCommit())
    return;

  if (frame_->IsLocalRoot()) {
    frame_->GetPage()->GetChromeClient().SetEventListenerProperties(
        frame_, WebEventListenerClass::kTouchStartOrMove,
        WebEventListenerProperties::kNothing);
    frame_->GetPage()->GetChromeClient().SetEventListenerProperties(
        frame_, WebEventListenerClass::kMouseWheel,
        WebEventListenerProperties::kNothing);
    frame_->GetPage()->GetChromeClient().SetEventListenerProperties(
        frame_, WebEventListenerClass::kTouchEndOrCancel,
        WebEventListenerProperties::kNothing);
  }

  Client()->TransitionToCommittedForNewPage();

  frame_->GetNavigationScheduler().Cancel();
}
