void Document::SetContent(const String& content) {
  if (ScriptableDocumentParser* parser = GetScriptableDocumentParser()) {
    if (parser->IsParsing() && parser->IsExecutingScript())
      return;
  }
  if (ignore_opens_during_unload_count_)
    return;

  open();
  parser_->Append(content);
  close();
}
