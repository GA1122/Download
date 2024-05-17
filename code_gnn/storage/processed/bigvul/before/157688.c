  const GURL GetLastNavigationURL() {
    NavigationRequest* navigation_request =
        contents()->GetFrameTree()->root()->navigation_request();
    CHECK(navigation_request);
    return navigation_request->common_params().url;
  }
