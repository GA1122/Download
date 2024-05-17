void WorkerFetchContext::AddResourceTiming(const ResourceTimingInfo& info) {
  if (global_scope_->IsWorkletGlobalScope())
    return;
  WorkerGlobalScopePerformance::performance(*ToWorkerGlobalScope(global_scope_))
      ->AddResourceTiming(info);
}
