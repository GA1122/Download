void Document::open() {
  DCHECK(!ImportLoader());
  DCHECK(!ignore_opens_during_unload_count_);
  if (ScriptableDocumentParser* parser = GetScriptableDocumentParser())
    DCHECK(!parser->IsParsing() || !parser->IsExecutingScript());

  if (frame_ &&
      (frame_->Loader().HasProvisionalNavigation() ||
       frame_->GetNavigationScheduler().IsNavigationScheduledWithin(0))) {
    frame_->Loader().StopAllLoaders();
    if (frame_ && frame_->Client())
      frame_->Client()->AbortClientNavigation();
  }
  javascript_url_task_handle_.Cancel();

  RemoveAllEventListenersRecursively();

  ResetTreeScope();
  if (frame_)
    frame_->Selection().Clear();

  ImplicitOpen(kForceSynchronousParsing);

  if (ScriptableDocumentParser* parser = GetScriptableDocumentParser())
    parser->SetWasCreatedByScript(true);

  if (frame_)
    frame_->Loader().DidExplicitOpen();
}
