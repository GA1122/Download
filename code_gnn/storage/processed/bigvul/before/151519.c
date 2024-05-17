WorkerFetchContext::WorkerFetchContext(
    WorkerOrWorkletGlobalScope& global_scope,
    std::unique_ptr<WebWorkerFetchContext> web_context)
    : global_scope_(global_scope),
      web_context_(std::move(web_context)),
      loading_task_runner_(
          TaskRunnerHelper::Get(TaskType::kUnspecedLoading, global_scope_)) {
  web_context_->InitializeOnWorkerThread(
      loading_task_runner_->ToSingleThreadTaskRunner());
  std::unique_ptr<blink::WebDocumentSubresourceFilter> web_filter =
      web_context_->TakeSubresourceFilter();
  if (web_filter) {
    subresource_filter_ =
        SubresourceFilter::Create(global_scope, std::move(web_filter));
  }
}
