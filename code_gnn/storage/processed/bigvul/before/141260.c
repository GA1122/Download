void Document::SuppressLoadEvent() {
  if (!LoadEventFinished())
    load_event_progress_ = kLoadEventCompleted;
}
