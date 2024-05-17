  AppCacheStorageImplTest() {
    auto head = network::CreateResourceResponseHead(net::HTTP_OK);
    network::URLLoaderCompletionStatus status;

    head.mime_type = "text/cache-manifest";
    mock_url_loader_factory_.AddResponse(GetMockUrl("manifest"), head,
                                         "CACHE MANIFEST\n", status);

    head.mime_type = "text/html";
    mock_url_loader_factory_.AddResponse(GetMockUrl("empty.html"), head, "",
                                         status);
  }
