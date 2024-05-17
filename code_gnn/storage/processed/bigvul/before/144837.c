   void PrepareTabs(const char* url1 = kTestUrl,
                   const char* url2 = kTestUrl,
                   const char* url3 = kTestUrl) {
    contents1_ = CreateTestWebContents();
    ResourceCoordinatorTabHelper::CreateForWebContents(contents1_.get());
    nav_handle1_ = CreateTabAndNavigation(url1, contents1_.get());
    contents2_ = CreateTestWebContents();
    ResourceCoordinatorTabHelper::CreateForWebContents(contents2_.get());
    nav_handle2_ = CreateTabAndNavigation(url2, contents2_.get());
    contents3_ = CreateTestWebContents();
    ResourceCoordinatorTabHelper::CreateForWebContents(contents3_.get());
    nav_handle3_ = CreateTabAndNavigation(url3, contents3_.get());

    contents1_->WasHidden();
    contents2_->WasHidden();
    contents3_->WasHidden();

    throttle1_ = std::make_unique<NonResumingBackgroundTabNavigationThrottle>(
        nav_handle1_.get());
    throttle2_ = std::make_unique<NonResumingBackgroundTabNavigationThrottle>(
        nav_handle2_.get());
    throttle3_ = std::make_unique<NonResumingBackgroundTabNavigationThrottle>(
        nav_handle3_.get());
  }
