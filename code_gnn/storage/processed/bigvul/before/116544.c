  void NavigateActiveAndCommit(const GURL& url) {
    controller().LoadURL(
        url, content::Referrer(), content::PAGE_TRANSITION_LINK, std::string());
    TestRenderViewHost* old_rvh = test_rvh();

    if (old_rvh != active_rvh())
      old_rvh->SendShouldCloseACK(true);

    int32 max_page_id = contents()->GetMaxPageIDForSiteInstance(
        active_rvh()->GetSiteInstance());
    active_test_rvh()->SendNavigate(max_page_id + 1, url);

    if (old_rvh != active_rvh())
      old_rvh->OnSwapOutACK();
  }
