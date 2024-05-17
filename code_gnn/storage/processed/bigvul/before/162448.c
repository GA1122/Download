void DocumentModuleScriptFetcher::Fetch(FetchParameters& fetch_params,
                                        ModuleScriptFetcher::Client* client) {
  SetClient(client);
  ScriptResource::Fetch(fetch_params, fetcher_, this);
}
