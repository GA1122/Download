  void TestWebUISubframeNewWindowToWebAllowed(int bindings) {
    FrameTreeNode* root = static_cast<WebContentsImpl*>(shell()->web_contents())
                              ->GetFrameTree()
                              ->root();

    GURL chrome_url = GURL(std::string(kChromeUIScheme) + "://" +
                           std::string(kChromeUIBlobInternalsHost));
    EXPECT_TRUE(NavigateToURL(shell(), chrome_url));
    RenderFrameHost* webui_rfh = root->current_frame_host();
    scoped_refptr<SiteInstance> webui_site_instance =
        webui_rfh->GetSiteInstance();

    ChildProcessSecurityPolicyImpl::GetInstance()->GrantWebUIBindings(
        webui_rfh->GetProcess()->GetID(), bindings);

    EXPECT_EQ(chrome_url, webui_rfh->GetLastCommittedURL());
    EXPECT_TRUE(ChildProcessSecurityPolicyImpl::GetInstance()->HasWebUIBindings(
        webui_rfh->GetProcess()->GetID()));

    {
      std::string script = base::StringPrintf(
          "var frame = document.createElement('iframe');\n"
          "frame.src = '%s';\n"
          "document.body.appendChild(frame);\n",
          chrome_url.spec().c_str());

      TestNavigationObserver navigation_observer(shell()->web_contents());
      EXPECT_TRUE(ExecuteScript(shell(), script));
      navigation_observer.Wait();

      EXPECT_EQ(1U, root->child_count());
      EXPECT_TRUE(navigation_observer.last_navigation_succeeded());
    }

    GURL web_url(embedded_test_server()->GetURL("/title2.html"));
    std::string script = base::StringPrintf(
        "var a = document.createElement('a');"
        "a.href = '%s'; a.target = '_blank'; a.click()",
        web_url.spec().c_str());

    ShellAddedObserver new_shell_observer;
    EXPECT_TRUE(ExecuteScript(root->child_at(0)->current_frame_host(), script));
    Shell* new_shell = new_shell_observer.GetShell();
    WaitForLoadStop(new_shell->web_contents());

    EXPECT_EQ(web_url, new_shell->web_contents()->GetLastCommittedURL());

  }
