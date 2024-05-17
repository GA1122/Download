void DoReplaceStateWhilePending(Shell* shell,
                                const GURL& start_url,
                                const GURL& stalled_url,
                                const std::string& replace_state_filename) {
  NavigationControllerImpl& controller =
      static_cast<NavigationControllerImpl&>(
          shell->web_contents()->GetController());

  FrameTreeNode* root =
      static_cast<WebContentsImpl*>(shell->web_contents())->
          GetFrameTree()->root();

  EXPECT_TRUE(NavigateToURL(shell, start_url));

  TestNavigationManager stalled_navigation(shell->web_contents(), stalled_url);
  controller.LoadURL(stalled_url, Referrer(), ui::PAGE_TRANSITION_LINK,
                     std::string());
  EXPECT_TRUE(stalled_navigation.WaitForRequestStart());

  NavigationEntryImpl* entry = controller.GetPendingEntry();
  ASSERT_NE(nullptr, entry);
  EXPECT_EQ(stalled_url, entry->GetURL());

  {
    FrameNavigateParamsCapturer capturer(root);
    capturer.set_wait_for_load(false);
    std::string script =
        "history.replaceState({}, '', '" + replace_state_filename + "')";
    EXPECT_TRUE(ExecJs(root, script));
    capturer.Wait();

    EXPECT_EQ(NAVIGATION_TYPE_EXISTING_PAGE, capturer.navigation_type());
    EXPECT_TRUE(capturer.is_same_document());
  }
}
