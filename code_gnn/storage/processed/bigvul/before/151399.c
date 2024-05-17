std::unique_ptr<WebURLLoader> FrameFetchContext::CreateURLLoader(
    const ResourceRequest& request,
    WebTaskRunner* task_runner) {
  DCHECK(!IsDetached());
  if (MasterDocumentLoader()->GetServiceWorkerNetworkProvider()) {
    WrappedResourceRequest webreq(request);
    auto loader =
        MasterDocumentLoader()
            ->GetServiceWorkerNetworkProvider()
            ->CreateURLLoader(webreq, task_runner->ToSingleThreadTaskRunner());
    if (loader)
      return loader;
  }

  return GetFrame()->CreateURLLoader(request, task_runner);
}
