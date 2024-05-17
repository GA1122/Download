scoped_ptr<ProxyResolverV8Tracing> CreateResolver(
    scoped_ptr<ProxyResolverV8Tracing::Bindings> bindings,
    const char* filename) {
  scoped_ptr<ProxyResolverV8Tracing> resolver;
  scoped_ptr<ProxyResolverV8TracingFactory> factory(
      ProxyResolverV8TracingFactory::Create());
  TestCompletionCallback callback;
  scoped_ptr<ProxyResolverFactory::Request> request;
  factory->CreateProxyResolverV8Tracing(LoadScriptData(filename),
                                        std::move(bindings), &resolver,
                                        callback.callback(), &request);
  EXPECT_EQ(OK, callback.WaitForResult());
  EXPECT_TRUE(resolver);
  return resolver;
}
