void PPAPITestBase::RunTestViaHTTP(const std::string& test_case) {
  FilePath document_root;
  ASSERT_TRUE(GetHTTPDocumentRoot(&document_root));
  RunHTTPTestServer(document_root, test_case, "");
}
