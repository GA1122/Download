void Document::DidRemoveAllPendingBodyStylesheets() {
  if (ScriptableDocumentParser* parser = GetScriptableDocumentParser())
    parser->DidLoadAllBodyStylesheets();
}
