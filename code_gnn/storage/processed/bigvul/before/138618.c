  TestNavigationURLLoader* GetLoaderForNavigationRequest(
      NavigationRequest* request) const {
    return static_cast<TestNavigationURLLoader*>(request->loader_for_testing());
  }
