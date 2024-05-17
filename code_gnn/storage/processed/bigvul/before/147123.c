void DocumentLoader::EnsureWriter(const AtomicString& mime_type,
                                  const KURL& overriding_url) {
  if (writer_)
    return;

  if (!GetFrameLoader().StateMachine()->CreatingInitialEmptyDocument()) {
    SetHistoryItemStateForCommit(
        GetFrameLoader().GetDocumentLoader()->GetHistoryItem(), load_type_,
        HistoryNavigationType::kDifferentDocument);
  }

  DCHECK_EQ(state_, kProvisional);
  GetFrameLoader().CommitProvisionalLoad();
  if (!frame_)
    return;

  const AtomicString& encoding = GetResponse().TextEncodingName();

  Document* owner_document = nullptr;
  if (Document::ShouldInheritSecurityOriginFromOwner(Url())) {
    Frame* owner_frame = frame_->Tree().Parent();
    if (!owner_frame)
      owner_frame = frame_->Loader().Opener();
    if (owner_frame && owner_frame->IsLocalFrame())
      owner_document = ToLocalFrame(owner_frame)->GetDocument();
  }
  bool should_reuse_default_view = frame_->ShouldReuseDefaultView(Url());
  DCHECK(frame_->GetPage());

  ParserSynchronizationPolicy parsing_policy = kAllowAsynchronousParsing;
  if ((substitute_data_.IsValid() && substitute_data_.ForceSynchronousLoad()) ||
      !Document::ThreadedParsingEnabledForTesting())
    parsing_policy = kForceSynchronousParsing;

  InstallNewDocument(Url(), owner_document, should_reuse_default_view,
                     mime_type, encoding, InstallNewDocumentReason::kNavigation,
                     parsing_policy, overriding_url);
  writer_->SetDocumentWasLoadedAsPartOfNavigation();
  frame_->GetDocument()->MaybeHandleHttpRefresh(
      response_.HttpHeaderField(HTTPNames::Refresh),
      Document::kHttpRefreshFromHeader);
}
