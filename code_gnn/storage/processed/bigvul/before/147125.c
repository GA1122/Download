void DocumentLoader::LoadFailed(const ResourceError& error) {
  if (!error.IsCancellation() && frame_->Owner()) {
    if (frame_->Owner()->IsLocal())
      frame_->DeprecatedLocalOwner()->RenderFallbackContent();
  }
  HistoryCommitType history_commit_type = LoadTypeToCommitType(load_type_);
  switch (state_) {
    case kNotStarted:
      probe::frameClearedScheduledClientNavigation(frame_);
    case kProvisional:
      state_ = kSentDidFinishLoad;
      frame_->FrameScheduler()->DidFailProvisionalLoad();
      GetLocalFrameClient().DispatchDidFailProvisionalLoad(error,
                                                           history_commit_type);
      if (frame_)
        GetFrameLoader().DetachProvisionalDocumentLoader(this);
      break;
    case kCommitted:
      if (frame_->GetDocument()->Parser())
        frame_->GetDocument()->Parser()->StopParsing();
      state_ = kSentDidFinishLoad;
      GetLocalFrameClient().DispatchDidFailLoad(error, history_commit_type);
      if (frame_)
        frame_->GetDocument()->CheckCompleted();
      break;
    case kSentDidFinishLoad:
      GetFrameLoader().DidFinishNavigation();
      break;
  }
  DCHECK_EQ(kSentDidFinishLoad, state_);
}
