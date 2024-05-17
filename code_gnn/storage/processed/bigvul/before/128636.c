void ContextualSearchDelegate::StartSearchTermRequestFromSelection(
    const base::string16& surrounding_text,
    int start_offset,
    int end_offset) {
  if (context_.get()) {
    SaveSurroundingText(surrounding_text, start_offset, end_offset);
    SendSurroundingText(kSurroundingSizeForUI);
    ContinueSearchTermResolutionRequest();
  } else {
    DVLOG(1) << "ctxs: Null context, ignored!";
  }
}
