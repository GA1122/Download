ExtensionApiTest::ExtensionApiTest() {
  embedded_test_server()->RegisterRequestHandler(
      base::Bind(&HandleServerRedirectRequest));
  embedded_test_server()->RegisterRequestHandler(
      base::Bind(&HandleEchoHeaderRequest));
  embedded_test_server()->RegisterRequestHandler(
      base::Bind(&HandleSetCookieRequest));
  embedded_test_server()->RegisterRequestHandler(
      base::Bind(&HandleSetHeaderRequest));
}
