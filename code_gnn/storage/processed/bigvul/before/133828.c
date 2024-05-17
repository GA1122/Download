void CreateProxyResolverFactory(
    mojo::InterfaceRequest<net::interfaces::ProxyResolverFactory> request) {
  new net::MojoProxyResolverFactoryImpl(request.Pass());
}
