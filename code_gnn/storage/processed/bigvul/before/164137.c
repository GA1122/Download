  AppCacheServiceImplTest()
      : kOriginURL("http://hello/"),
        kOrigin(url::Origin::Create(kOriginURL)),
        kManifestUrl(kOriginURL.Resolve("manifest")),
        service_(new AppCacheServiceImpl(nullptr)),
        delete_result_(net::OK),
        delete_completion_count_(0) {
    service_->storage_.reset(new MockAppCacheStorage(service_.get()));
  }
