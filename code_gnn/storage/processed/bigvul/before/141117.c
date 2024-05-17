DocumentParser* Document::ImplicitOpen(
    ParserSynchronizationPolicy parser_sync_policy) {
  RemoveChildren();
  DCHECK(!focused_element_);

  SetCompatibilityMode(kNoQuirksMode);

  if (!ThreadedParsingEnabledForTesting()) {
    parser_sync_policy = kForceSynchronousParsing;
  } else if (parser_sync_policy == kAllowAsynchronousParsing &&
             IsPrefetchOnly()) {
    parser_sync_policy = kForceSynchronousParsing;
  }

  DetachParser();
  parser_sync_policy_ = parser_sync_policy;
  parser_ = CreateParser();
  DocumentParserTiming::From(*this).MarkParserStart();
  SetParsingState(kParsing);
  SetReadyState(kLoading);
  if (load_event_progress_ != kLoadEventInProgress &&
      PageDismissalEventBeingDispatched() == kNoDismissal) {
    load_event_progress_ = kLoadEventNotRun;
  }

  return parser_;
}
