  void TestWebFrameInWebUIProcessDisallowed(int bindings) {
    FrameTreeNode* root = static_cast<WebContentsImpl*>(shell()->web_contents())
                              ->GetFrameTree()
                              ->root();
    GURL data_url("data:text/html,a data url document");
    EXPECT_TRUE(NavigateToURL(shell(), data_url));
    EXPECT_EQ(data_url, root->current_frame_host()->GetLastCommittedURL());
    EXPECT_FALSE(
        ChildProcessSecurityPolicyImpl::GetInstance()->HasWebUIBindings(
            root->current_frame_host()->GetProcess()->GetID()));

    ChildProcessSecurityPolicyImpl::GetInstance()->GrantWebUIBindings(
        root->current_frame_host()->GetProcess()->GetID(), bindings);
    EXPECT_TRUE(ChildProcessSecurityPolicyImpl::GetInstance()->HasWebUIBindings(
        root->current_frame_host()->GetProcess()->GetID()));
    {
      GURL web_url(embedded_test_server()->GetURL("/title2.html"));
      std::string script = base::StringPrintf(
          "var frame = document.createElement('iframe');\n"
          "frame.src = '%s';\n"
          "document.body.appendChild(frame);\n",
          web_url.spec().c_str());

      TestNavigationObserver navigation_observer(shell()->web_contents());
      EXPECT_TRUE(ExecuteScript(shell(), script));
      navigation_observer.Wait();

      EXPECT_EQ(1U, root->child_count());
      EXPECT_FALSE(navigation_observer.last_navigation_succeeded());
    }
  }
