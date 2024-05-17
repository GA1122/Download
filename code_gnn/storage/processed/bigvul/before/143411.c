 void DnsDuringInitHelper(bool synchronous_host_resolver) {
   MockCachingHostResolver host_resolver;
   MockBindings mock_bindings(&host_resolver);
  host_resolver.set_synchronous_mode(synchronous_host_resolver);

  host_resolver.rules()->AddRule("host1", "91.13.12.1");
  host_resolver.rules()->AddRule("host2", "91.13.12.2");

  scoped_ptr<ProxyResolverV8Tracing> resolver =
      CreateResolver(mock_bindings.CreateBindings(), "dns_during_init.js");

  EXPECT_EQ(2u, host_resolver.num_resolve());

  host_resolver.rules()->ClearRules();
  host_resolver.GetHostCache()->clear();

  host_resolver.rules()->AddRule("host1", "145.88.13.3");
  host_resolver.rules()->AddRule("host2", "137.89.8.45");

  TestCompletionCallback callback;
  ProxyInfo proxy_info;

  resolver->GetProxyForURL(GURL("http://foo/"), &proxy_info,
                           callback.callback(), NULL,
                           mock_bindings.CreateBindings());
  EXPECT_EQ(OK, callback.WaitForResult());

  EXPECT_EQ(4u, host_resolver.num_resolve());

  EXPECT_EQ("91.13.12.1-91.13.12.2-145.88.13.3-137.89.8.45:99",
            proxy_info.proxy_server().ToURI());

  ASSERT_EQ(2u, mock_bindings.GetAlerts().size());
  EXPECT_EQ("Watsup", mock_bindings.GetAlerts()[0]);
  EXPECT_EQ("Watsup2", mock_bindings.GetAlerts()[1]);
}
