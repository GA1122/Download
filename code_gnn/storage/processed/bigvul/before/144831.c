  std::unique_ptr<NavigationHandle> CreateTabAndNavigation(
      const char* url,
      content::WebContents* web_contents) {
    TabUIHelper::CreateForWebContents(web_contents);
    return content::NavigationHandle::CreateNavigationHandleForTesting(
        GURL(url), web_contents->GetMainFrame());
  }
