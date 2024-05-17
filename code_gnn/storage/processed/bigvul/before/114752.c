void PPAPITestBase::RunTestWithWebSocketServer(const std::string& test_case) {
  FilePath websocket_root_dir;
  ASSERT_TRUE(
      PathService::Get(content::DIR_LAYOUT_TESTS, &websocket_root_dir));
  ui_test_utils::TestWebSocketServer server;
  int port = server.UseRandomPort();
  ASSERT_TRUE(server.Start(websocket_root_dir));
  FilePath http_document_root;
  ASSERT_TRUE(GetHTTPDocumentRoot(&http_document_root));
  RunHTTPTestServer(http_document_root, test_case,
                    StringPrintf("websocket_port=%d", port));
}
