  void TestTransitionFromActiveToPendingFreeze() {
    content::SetupCrossSiteRedirector(embedded_test_server());
    ASSERT_TRUE(embedded_test_server()->Start());

    GURL main_url(
        embedded_test_server()->GetURL("a.com", "/iframe_cross_site.html"));
    OpenTwoTabs(GURL(chrome::kChromeUIAboutURL), main_url);
    constexpr int kFreezingIndex = 1;
    LifecycleUnit* const lifecycle_unit = GetLifecycleUnitAt(kFreezingIndex);
    content::WebContents* const content = GetWebContentsAt(kFreezingIndex);

    content::RenderFrameHost* main_frame = content->GetMainFrame();
    ASSERT_EQ(3u, content->GetAllFrames().size());
    content::RenderFrameHost* child_frame = content->GetAllFrames()[1];
    EXPECT_FALSE(content::SiteInstance::IsSameWebSite(
        browser()->profile(), main_frame->GetLastCommittedURL(),
        child_frame->GetLastCommittedURL()));
    if (content::AreAllSitesIsolatedForTesting()) {
      EXPECT_NE(main_frame->GetProcess()->GetID(),
                child_frame->GetProcess()->GetID());
    }

    bool hidden_state_result;
    EXPECT_TRUE(content::ExecuteScriptAndExtractBool(
        main_frame,
        "window.domAutomationController.send("
        "window.document.hidden);",
        &hidden_state_result));
    EXPECT_TRUE(hidden_state_result);

    EXPECT_TRUE(content::ExecuteScript(
        main_frame,
        "if (window.location.pathname != '/iframe_cross_site.html')"
        "  throw 'Incorrect frame';"
        "mainFrameFreezeCount = 0;"
        "window.document.onfreeze = function(){ mainFrameFreezeCount++; };"));

    EXPECT_TRUE(content::ExecuteScript(
        child_frame,
        "if (window.location.pathname != '/title1.html') throw 'Incorrect "
        "frame';"
        "childFrameFreezeCount = 0;"
        "window.document.onfreeze = function(){ childFrameFreezeCount++; };"));

    int freeze_count_result;
    EXPECT_TRUE(content::ExecuteScriptAndExtractInt(
        main_frame, kMainFrameFrozenStateJS, &freeze_count_result));
    EXPECT_EQ(0, freeze_count_result);
    EXPECT_TRUE(content::ExecuteScriptAndExtractInt(
        child_frame, kChildFrameFrozenStateJS, &freeze_count_result));
    EXPECT_EQ(0, freeze_count_result);

    EXPECT_EQ(LifecycleUnitState::ACTIVE, lifecycle_unit->GetState());
    EXPECT_TRUE(lifecycle_unit->Freeze());
    EXPECT_EQ(LifecycleUnitState::PENDING_FREEZE, lifecycle_unit->GetState());
  }
