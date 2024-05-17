void DocumentModuleScriptFetcher::Trace(blink::Visitor* visitor) {
  visitor->Trace(fetcher_);
  ResourceClient::Trace(visitor);
  ModuleScriptFetcher::Trace(visitor);
}
