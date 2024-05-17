void Document::CancelParsing() {
  DetachParser();
  SetParsingState(kFinishedParsing);
  SetReadyState(kComplete);
  SuppressLoadEvent();
  javascript_url_task_handle_.Cancel();
}
