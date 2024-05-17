  size_t GetWorkerRefCount(const GURL& origin) {
    content::ServiceWorkerContext* sw_context =
        content::BrowserContext::GetDefaultStoragePartition(
            browser()->profile())
            ->GetServiceWorkerContext();
    base::RunLoop run_loop;
    size_t ref_count = 0;
    auto set_ref_count = [](size_t* ref_count, base::RunLoop* run_loop,
                            size_t external_request_count) {
      *ref_count = external_request_count;
      run_loop->Quit();
    };
    sw_context->CountExternalRequestsForTest(
        origin, base::BindOnce(set_ref_count, &ref_count, &run_loop));
    run_loop.Run();
    return ref_count;
  }
