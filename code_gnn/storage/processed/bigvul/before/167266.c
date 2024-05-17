FetchManager::Loader::Loader(ExecutionContext* execution_context,
                             FetchManager* fetch_manager,
                             ScriptPromiseResolver* resolver,
                             FetchRequestData* fetch_request_data,
                             bool is_isolated_world,
                             AbortSignal* signal)
    : fetch_manager_(fetch_manager),
      resolver_(resolver),
      fetch_request_data_(fetch_request_data),
      failed_(false),
      finished_(false),
      response_http_status_code_(0),
      integrity_verifier_(nullptr),
      did_finish_loading_(false),
      is_isolated_world_(is_isolated_world),
      signal_(signal),
      execution_context_(execution_context) {
  url_list_.push_back(fetch_request_data->Url());
}
