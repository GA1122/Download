  void Continue_Reinitialize(ReinitTestCase test_case) {
    const int kMockProcessId = 1;
    const int kMockRenderFrameId = MSG_ROUTING_NONE;
    backend_ =
        std::make_unique<AppCacheBackendImpl>(service_.get(), kMockProcessId);

    if (test_case == CORRUPT_SQL_ON_INSTALL) {
      EXPECT_FALSE(database()->was_corruption_detected());
      ASSERT_TRUE(sql::test::CorruptSizeInHeader(
          temp_directory_.GetPath().AppendASCII("Index")));
    }

    if (test_case == CORRUPT_CACHE_ON_INSTALL ||
        test_case == CORRUPT_SQL_ON_INSTALL) {
      backend_->RegisterHost(blink::mojom::AppCacheHostRequest(),
                             BindFrontend(), 1, kMockRenderFrameId);
      AppCacheHost* host1 = backend_->GetHost(1);
      const GURL kEmptyPageUrl(GetMockUrl("empty.html"));
      host1->SetFirstPartyUrlForTesting(kEmptyPageUrl);
      host1->SelectCache(kEmptyPageUrl, blink::mojom::kAppCacheNoCacheId,
                         GetMockUrl("manifest"));
    } else {
      ASSERT_EQ(CORRUPT_CACHE_ON_LOAD_EXISTING, test_case);
      backend_->RegisterHost(blink::mojom::AppCacheHostRequest(),
                             BindFrontend(), 2, kMockRenderFrameId);
      AppCacheHost* host2 = backend_->GetHost(2);
      network::ResourceRequest request;
      request.url = GetMockUrl("manifest");
      handler_ =
          host2->CreateRequestHandler(AppCacheURLLoaderRequest::Create(request),
                                      RESOURCE_TYPE_MAIN_FRAME, false);
      handler_->MaybeCreateLoader(request, nullptr, base::DoNothing(),
                                  base::DoNothing());
    }

    PushNextTask(base::BindOnce(&AppCacheStorageImplTest::Verify_Reinitialized,
                                base::Unretained(this), test_case));
  }
