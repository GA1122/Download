  void SimulateNavigation() {
    content::RenderFrameHost* rfh = web_contents()->GetMainFrame();
    std::unique_ptr<content::NavigationHandle> navigation_handle =
        content::NavigationHandle::CreateNavigationHandleForTesting(
            GURL(), rfh, true);
  }
