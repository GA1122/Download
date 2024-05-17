ScriptPromise FetchManager::Fetch(ScriptState* script_state,
                                  FetchRequestData* request,
                                  AbortSignal* signal) {
  ScriptPromiseResolver* resolver = ScriptPromiseResolver::Create(script_state);
  ScriptPromise promise = resolver->Promise();

  DCHECK(signal);
  if (signal->aborted()) {
    resolver->Reject(DOMException::Create(kAbortError));
    return promise;
  }

  request->SetContext(WebURLRequest::kRequestContextFetch);

  Loader* loader =
      Loader::Create(GetExecutionContext(), this, resolver, request,
                     script_state->World().IsIsolatedWorld(), signal);
  loaders_.insert(loader);
  signal->AddAlgorithm(WTF::Bind(&Loader::Abort, WrapWeakPersistent(loader)));
  loader->Start();
  return promise;
}
