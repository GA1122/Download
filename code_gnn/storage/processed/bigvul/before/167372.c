  RemoveSafeBrowsingCookieTester()
      : browser_process_(TestingBrowserProcess::GetGlobal()) {
    scoped_refptr<safe_browsing::SafeBrowsingService> sb_service =
        safe_browsing::SafeBrowsingService::CreateSafeBrowsingService();
    browser_process_->SetSafeBrowsingService(sb_service.get());
    sb_service->Initialize();
    base::RunLoop().RunUntilIdle();

    base::RunLoop run_loop;
    net::URLRequestContext* request_context =
        sb_service->url_request_context()->GetURLRequestContext();
    request_context->cookie_store()->DeleteAllAsync(
        base::BindOnce(&RunClosureAfterCookiesCleared, run_loop.QuitClosure()));
    run_loop.Run();

    SetCookieStore(request_context->cookie_store());
  }
