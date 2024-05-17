void Document::Abort() {
  CancelParsing();
  CheckCompletedInternal();
}
