void ExtensionWebRequestTest::FireURLRequestWithData(
    const std::string& method,
    const char* content_type,
    const std::vector<char>& bytes_1,
    const std::vector<char>& bytes_2) {
  GURL request_url("http://www.example.com");
  std::unique_ptr<net::URLRequest> request(
      context_->CreateRequest(request_url, net::DEFAULT_PRIORITY, &delegate_));
  request->set_method(method);
  if (content_type != NULL) {
    request->SetExtraRequestHeaderByName(net::HttpRequestHeaders::kContentType,
                                         content_type,
                                         true  );
  }
  std::vector<std::unique_ptr<net::UploadElementReader>> element_readers;
  element_readers.push_back(base::MakeUnique<net::UploadBytesElementReader>(
      &(bytes_1[0]), bytes_1.size()));
  element_readers.push_back(base::MakeUnique<net::UploadFileElementReader>(
      base::ThreadTaskRunnerHandle::Get().get(), base::FilePath(), 0, 0,
      base::Time()));
  element_readers.push_back(base::MakeUnique<net::UploadBytesElementReader>(
      &(bytes_2[0]), bytes_2.size()));
  request->set_upload(base::MakeUnique<net::ElementsUploadDataStream>(
      std::move(element_readers), 0));
  ipc_sender_.PushTask(base::Bind(&base::DoNothing));
  request->Start();
}
