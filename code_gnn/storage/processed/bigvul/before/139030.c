static inline bool pageIsBeingDismissed(Document* document) {
  return document->pageDismissalEventBeingDispatched() != Document::NoDismissal;
}
