void PPAPITestBase::RunHTTPTestServer(
    const FilePath& document_root,
    const std::string& test_case,
    const std::string& extra_params) {
  net::TestServer test_server(net::TestServer::TYPE_HTTP,
                              net::TestServer::kLocalhost,
                              document_root);
  ASSERT_TRUE(test_server.Start());
  std::string query = BuildQuery("files/test_case.html?", test_case);
  if (!extra_params.empty())
    query = StringPrintf("%s&%s", query.c_str(), extra_params.c_str());

  GURL url = test_server.GetURL(query);
  RunTestURL(url);
}
