void SetupCrossSiteRedirector(net::EmbeddedTestServer* embedded_test_server) {
  embedded_test_server->RegisterRequestHandler(
      base::Bind(&CrossSiteRedirectResponseHandler, embedded_test_server));
}
