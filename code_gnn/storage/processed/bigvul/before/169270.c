int LoadBasicRequest(network::mojom::NetworkContext* network_context,
                     const GURL& url,
                     int process_id,
                     int render_frame_id) {
  network::mojom::URLLoaderFactoryPtr url_loader_factory;
  network_context->CreateURLLoaderFactory(MakeRequest(&url_loader_factory),
                                          process_id);
  EXPECT_FALSE(url_loader_factory.encountered_error());

  auto request = std::make_unique<network::ResourceRequest>();
  request->url = url;
  request->render_frame_id = render_frame_id;

  content::SimpleURLLoaderTestHelper simple_loader_helper;
  std::unique_ptr<network::SimpleURLLoader> simple_loader =
      network::SimpleURLLoader::Create(std::move(request),
                                       TRAFFIC_ANNOTATION_FOR_TESTS);

  simple_loader->DownloadToStringOfUnboundedSizeUntilCrashAndDie(
      url_loader_factory.get(), simple_loader_helper.GetCallback());
  simple_loader_helper.WaitForCallback();

  return simple_loader->NetError();
}
