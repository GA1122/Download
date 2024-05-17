void Document::CancelParsing() {
  DetachParser();
  SetParsingState(kFinishedParsing);
  SetReadyState(kComplete);
  SuppressLoadEvent();
}
