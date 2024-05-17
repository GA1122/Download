void HTMLMediaElement::SourceWasAdded(HTMLSourceElement* source) {
  BLINK_MEDIA_LOG << "sourceWasAdded(" << (void*)this << ", " << source << ")";

  KURL url = source->GetNonEmptyURLAttribute(kSrcAttr);
  BLINK_MEDIA_LOG << "sourceWasAdded(" << (void*)this << ") - 'src' is "
                  << UrlForLoggingMedia(url);

  if (FastHasAttribute(kSrcAttr))
    return;

  if (getNetworkState() == HTMLMediaElement::kNetworkEmpty) {
    InvokeResourceSelectionAlgorithm();
    next_child_node_to_consider_ = source;
    return;
  }

  if (current_source_node_ && source == current_source_node_->nextSibling()) {
    BLINK_MEDIA_LOG << "sourceWasAdded(" << (void*)this
                    << ") - <source> inserted immediately after current source";
    next_child_node_to_consider_ = source;
    return;
  }

  if (next_child_node_to_consider_)
    return;

  if (load_state_ != kWaitingForSource)
    return;

  SetShouldDelayLoadEvent(true);

  SetNetworkState(kNetworkLoading);

  next_child_node_to_consider_ = source;
  ScheduleNextSourceChild();
}
