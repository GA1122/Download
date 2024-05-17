RenderFrameImpl::CreateWorkerFetchContext() {
  blink::WebServiceWorkerNetworkProvider* web_provider =
      frame_->GetDocumentLoader()->GetServiceWorkerNetworkProvider();
  DCHECK(web_provider);
  ServiceWorkerNetworkProvider* provider =
      ServiceWorkerNetworkProvider::FromWebServiceWorkerNetworkProvider(
          web_provider);
  mojom::ServiceWorkerWorkerClientRequest service_worker_client_request;
  mojom::ServiceWorkerWorkerClientRegistryPtrInfo
      service_worker_worker_client_registry_ptr_info;
  mojom::ServiceWorkerContainerHostPtrInfo container_host_ptr_info;
  ServiceWorkerProviderContext* provider_context = provider->context();
  if (provider_context) {
    provider_context->CloneWorkerClientRegistry(
        mojo::MakeRequest(&service_worker_worker_client_registry_ptr_info));

    mojom::ServiceWorkerWorkerClientPtr worker_client_ptr;
    service_worker_client_request = mojo::MakeRequest(&worker_client_ptr);
    provider_context->RegisterWorkerClient(std::move(worker_client_ptr));

    if (blink::ServiceWorkerUtils::IsServicificationEnabled())
      container_host_ptr_info = provider_context->CloneContainerHostPtrInfo();
  }

  mojom::RendererPreferenceWatcherPtr watcher;
  mojom::RendererPreferenceWatcherRequest watcher_request =
      mojo::MakeRequest(&watcher);
  render_view()->RegisterRendererPreferenceWatcherForWorker(std::move(watcher));

  auto worker_fetch_context = base::MakeRefCounted<WebWorkerFetchContextImpl>(
      render_view_->renderer_preferences(), std::move(watcher_request),
      std::move(service_worker_client_request),
      std::move(service_worker_worker_client_registry_ptr_info),
      std::move(container_host_ptr_info), GetLoaderFactoryBundle()->Clone(),
      GetLoaderFactoryBundle()->CloneWithoutAppCacheFactory(),
      GetContentClient()->renderer()->CreateURLLoaderThrottleProvider(
          URLLoaderThrottleProviderType::kWorker),
      GetContentClient()
          ->renderer()
          ->CreateWebSocketHandshakeThrottleProvider(),
      ChildThreadImpl::current()->thread_safe_sender(),
      ChildThreadImpl::current()->GetConnector()->Clone());

  worker_fetch_context->set_ancestor_frame_id(routing_id_);
  worker_fetch_context->set_frame_request_blocker(frame_request_blocker_);
  worker_fetch_context->set_site_for_cookies(
      frame_->GetDocument().SiteForCookies());
  worker_fetch_context->set_is_secure_context(
      frame_->GetDocument().IsSecureContext());
  worker_fetch_context->set_service_worker_provider_id(provider->provider_id());
  worker_fetch_context->set_is_controlled_by_service_worker(
      provider->IsControlledByServiceWorker());
  worker_fetch_context->set_origin_url(
      GURL(frame_->GetDocument().Url()).GetOrigin());
  if (provider_context)
    worker_fetch_context->set_client_id(provider_context->client_id());

  for (auto& observer : observers_)
    observer.WillCreateWorkerFetchContext(worker_fetch_context.get());
  return std::move(worker_fetch_context);
}
