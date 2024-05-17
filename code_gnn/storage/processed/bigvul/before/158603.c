std::unique_ptr<WebURLLoader> FrameFetchContext::CreateURLLoader(
    const ResourceRequest& request,
    const ResourceLoaderOptions& options) {
  DCHECK(!IsDetached());
  WrappedResourceRequest webreq(request);

  network::mojom::blink::URLLoaderFactoryPtr url_loader_factory;
  if (options.url_loader_factory) {
    options.url_loader_factory->data->Clone(MakeRequest(&url_loader_factory));
  }
  if (document_ && request.Url().ProtocolIs("blob") &&
      BlobUtils::MojoBlobURLsEnabled() && !url_loader_factory &&
      request.GetRequestContext() !=
          WebURLRequest::kRequestContextSharedWorker) {
    document_->GetPublicURLManager().Resolve(request.Url(),
                                             MakeRequest(&url_loader_factory));
  }
  if (url_loader_factory) {
    return Platform::Current()
        ->WrapURLLoaderFactory(url_loader_factory.PassInterface().PassHandle())
        ->CreateURLLoader(webreq, CreateResourceLoadingTaskRunnerHandle());
  }

  if (MasterDocumentLoader()->GetServiceWorkerNetworkProvider()) {
    auto loader =
        MasterDocumentLoader()
            ->GetServiceWorkerNetworkProvider()
            ->CreateURLLoader(webreq, CreateResourceLoadingTaskRunnerHandle());
    if (loader)
      return loader;
  }
  return GetFrame()->GetURLLoaderFactory()->CreateURLLoader(
      webreq, CreateResourceLoadingTaskRunnerHandle());
}
