  static Loader* Create(ExecutionContext* execution_context,
                        FetchManager* fetch_manager,
                        ScriptPromiseResolver* resolver,
                        FetchRequestData* request,
                        bool is_isolated_world,
                        AbortSignal* signal) {
    return new Loader(execution_context, fetch_manager, resolver, request,
                      is_isolated_world, signal);
  }
