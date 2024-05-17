DocumentParser* Document::CreateParser() {
  if (IsHTMLDocument())
    return HTMLDocumentParser::Create(ToHTMLDocument(*this),
                                      parser_sync_policy_);
  return XMLDocumentParser::Create(*this, View());
}
