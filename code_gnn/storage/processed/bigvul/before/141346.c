void Document::open(Document* entered_document,
                    ExceptionState& exception_state) {
  if (ImportLoader()) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kInvalidStateError,
        "Imported document doesn't support open().");
    return;
  }

  if (!IsHTMLDocument()) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      "Only HTML documents support open().");
    return;
  }

  if (throw_on_dynamic_markup_insertion_count_) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kInvalidStateError,
        "Custom Element constructor should not use open().");
    return;
  }

  if (!AllowedToUseDynamicMarkUpInsertion("open", exception_state))
    return;

  if (entered_document && !GetSecurityOrigin()->IsSameSchemeHostPort(
                              entered_document->GetSecurityOrigin())) {
    exception_state.ThrowSecurityError(
        "Can only call open() on same-origin documents.");
    return;
  }

  if (ScriptableDocumentParser* parser = GetScriptableDocumentParser()) {
    if (parser->IsParsing() && parser->IsExecutingScript())
      return;
  }

  if (ignore_opens_during_unload_count_)
    return;

  if (entered_document && this != entered_document) {
    KURL new_url = entered_document->Url();
    new_url.SetFragmentIdentifier(String());
    SetURL(new_url);

    SetSecurityOrigin(entered_document->GetMutableSecurityOrigin());
    SetReferrerPolicy(entered_document->GetReferrerPolicy());
    SetCookieURL(entered_document->CookieURL());
  }

  open();
}
