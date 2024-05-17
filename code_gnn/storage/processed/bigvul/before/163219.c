  void CheckHttpsUpgradedIframeNavigation(const GURL& start_url,
                                          const GURL& iframe_secure_url) {
    ASSERT_TRUE(start_url.SchemeIs(url::kHttpScheme));
    ASSERT_TRUE(iframe_secure_url.SchemeIs(url::kHttpsScheme));

    NavigationStartUrlRecorder url_recorder(shell()->web_contents());
    TestNavigationThrottleInstaller installer(
        shell()->web_contents(), NavigationThrottle::PROCEED,
        NavigationThrottle::PROCEED, NavigationThrottle::PROCEED,
        NavigationThrottle::PROCEED);
    TestNavigationManager navigation_manager(shell()->web_contents(),
                                             iframe_secure_url);

    shell()->LoadURL(start_url);
    EXPECT_TRUE(navigation_manager.WaitForRequestStart());

    EXPECT_EQ(2, installer.will_start_called());
    EXPECT_EQ(0, installer.will_redirect_called());
    EXPECT_EQ(1, installer.will_process_called());

    EXPECT_EQ(iframe_secure_url, url_recorder.urls().back());
    EXPECT_EQ(start_url, url_recorder.urls().front());
    EXPECT_EQ(2ul, url_recorder.urls().size());
  }
