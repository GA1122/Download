ProxyResolverV8TracingFactory::Create() {
   return make_scoped_ptr(new ProxyResolverV8TracingFactoryImpl());
 }
