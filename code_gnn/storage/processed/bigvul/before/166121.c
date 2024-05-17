bool RenderFrameHostImpl::CreateNetworkServiceDefaultFactoryInternal(
    const base::Optional<url::Origin>& origin,
    network::mojom::URLLoaderFactoryRequest default_factory_request) {
  auto* context = GetSiteInstance()->GetBrowserContext();
  bool bypass_redirect_checks = false;

  network::mojom::TrustedURLLoaderHeaderClientPtrInfo header_client;
  if (base::FeatureList::IsEnabled(network::features::kNetworkService)) {
    GetContentClient()->browser()->WillCreateURLLoaderFactory(
        context, this, GetProcess()->GetID(), false  ,
        origin.value_or(url::Origin()), &default_factory_request,
        &header_client, &bypass_redirect_checks);
  }

  devtools_instrumentation::WillCreateURLLoaderFactory(
      this, false  , false  ,
      &default_factory_request);

  if (g_create_network_factory_callback_for_test.Get().is_null()) {
    GetProcess()->CreateURLLoaderFactory(origin, std::move(header_client),
                                         std::move(default_factory_request));
  } else {
    network::mojom::URLLoaderFactoryPtr original_factory;
    GetProcess()->CreateURLLoaderFactory(origin, std::move(header_client),
                                         mojo::MakeRequest(&original_factory));
    g_create_network_factory_callback_for_test.Get().Run(
        std::move(default_factory_request), GetProcess()->GetID(),
        original_factory.PassInterface());
  }

  return bypass_redirect_checks;
}
