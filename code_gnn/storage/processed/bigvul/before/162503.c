ClassicPendingScript* ClassicPendingScript::Fetch(
    const KURL& url,
    Document& element_document,
    const ScriptFetchOptions& options,
    const WTF::TextEncoding& encoding,
    ScriptElementBase* element,
    FetchParameters::DeferOption defer) {
  FetchParameters params = options.CreateFetchParameters(
      url, element_document.GetSecurityOrigin(), encoding, defer);

  ClassicPendingScript* pending_script = new ClassicPendingScript(
      element, TextPosition(), ScriptSourceLocationType::kExternalFile, options,
      true  );

  pending_script->intervened_ =
      MaybeDisallowFetchForDocWrittenScript(params, element_document);

  ScriptResource::Fetch(params, element_document.Fetcher(), pending_script);
  pending_script->CheckState();
  return pending_script;
}
