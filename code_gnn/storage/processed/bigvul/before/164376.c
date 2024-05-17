bool ExtensionApiTest::InitializeEmbeddedTestServer() {
  if (!embedded_test_server()->InitializeAndListen())
    return false;

  if (test_config_) {
    test_config_->SetInteger(kEmbeddedTestServerPort,
                             embedded_test_server()->port());
  }

  return true;
}
