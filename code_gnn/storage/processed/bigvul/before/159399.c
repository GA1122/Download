bool ExtensionApiTest::StartWebSocketServer(
    const base::FilePath& root_directory,
    bool enable_basic_auth) {
  websocket_server_.reset(new net::SpawnedTestServer(
      net::SpawnedTestServer::TYPE_WS, root_directory));
  websocket_server_->set_websocket_basic_auth(enable_basic_auth);

  if (!websocket_server_->Start())
    return false;

  test_config_->SetInteger(kTestWebSocketPort,
                           websocket_server_->host_port_pair().port());

  return true;
}
