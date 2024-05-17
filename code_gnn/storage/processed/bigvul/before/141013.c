void Document::DidAddPendingStylesheetInBody() {
  if (ScriptableDocumentParser* parser = GetScriptableDocumentParser())
    parser->DidAddPendingStylesheetInBody();
}
