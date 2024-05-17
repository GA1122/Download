DEFINE_TRACE(WorkerFetchContext) {
  visitor->Trace(global_scope_);
  visitor->Trace(subresource_filter_);
  visitor->Trace(resource_fetcher_);
  BaseFetchContext::Trace(visitor);
}
