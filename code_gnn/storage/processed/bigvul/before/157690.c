  bool HasNavigationRequest() {
    return contents()->GetFrameTree()->root()->navigation_request() != nullptr;
  }
