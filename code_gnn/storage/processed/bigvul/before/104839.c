  void PrerenderTestURLImpl(
      const GURL& url,
      const std::deque<FinalStatus>& expected_final_status_queue,
      int total_navigations) {
    dest_url_ = url;

    std::vector<net::TestServer::StringPair> replacement_text;
    replacement_text.push_back(
        make_pair("REPLACE_WITH_PREFETCH_URL", dest_url_.spec()));
    std::string replacement_path;
    ASSERT_TRUE(net::TestServer::GetFilePathWithReplacements(
        "files/prerender/prerender_loader.html",
        replacement_text,
        &replacement_path));

    net::TestServer* src_server = test_server();
    scoped_ptr<net::TestServer> https_src_server;
    if (use_https_src_server_) {
      https_src_server.reset(
          new net::TestServer(net::TestServer::TYPE_HTTPS,
                              FilePath(FILE_PATH_LITERAL("chrome/test/data"))));
      ASSERT_TRUE(https_src_server->Start());
      src_server = https_src_server.get();
    }
    GURL src_url = src_server->GetURL(replacement_path);

    ASSERT_TRUE(prerender_manager());
    prerender_manager()->rate_limit_enabled_ = false;
    ASSERT_TRUE(prerender_contents_factory_ == NULL);
    prerender_contents_factory_ =
        new WaitForLoadPrerenderContentsFactory(total_navigations,
                                                expected_final_status_queue);
    prerender_manager()->SetPrerenderContentsFactory(
        prerender_contents_factory_);
    FinalStatus expected_final_status = expected_final_status_queue.front();

    browser()->OpenURL(src_url, GURL(), CURRENT_TAB, PageTransition::TYPED);

    TestPrerenderContents* prerender_contents = NULL;
    ui_test_utils::RunMessageLoop();

    prerender_contents =
        static_cast<TestPrerenderContents*>(
            prerender_manager()->FindEntry(dest_url_));

    switch (expected_final_status) {
      case FINAL_STATUS_USED: {
        ASSERT_TRUE(prerender_contents != NULL);

        if (call_javascript_) {
          bool prerender_test_result = false;
          ASSERT_TRUE(ui_test_utils::ExecuteJavaScriptAndExtractBool(
              prerender_contents->render_view_host(), L"",
              L"window.domAutomationController.send(DidPrerenderPass())",
              &prerender_test_result));
          EXPECT_TRUE(prerender_test_result);
        }
        break;
      }
      default:
        EXPECT_TRUE(prerender_contents == NULL);
        break;
    }
  }
