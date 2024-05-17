void ChromeContentUtilityClient::RegisterMojoServices(
    content::ServiceRegistry* registry) {
#if !defined(OS_ANDROID)
  registry->AddService<net::interfaces::ProxyResolverFactory>(
      base::Bind(CreateProxyResolverFactory));
  registry->AddService<ResourceUsageReporter>(
      base::Bind(CreateResourceUsageReporter));
#endif
}
