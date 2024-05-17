std::unique_ptr<WebURLLoader> WorkerFetchContext::CreateURLLoader(
    const ResourceRequest& request,
    WebTaskRunner* task_runner) {
  CountUsage(WebFeature::kOffMainThreadFetch);
  WrappedResourceRequest wrapped(request);
  return web_context_->CreateURLLoader(wrapped,
                                       task_runner->ToSingleThreadTaskRunner());
}
