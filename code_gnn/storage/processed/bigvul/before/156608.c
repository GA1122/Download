  content::WebContents* SimulateCtrlClick(content::WebContents* main_contents,
                                          const char* id_of_anchor_to_click) {
    content::WebContents* new_contents = nullptr;
    {
      content::WebContentsAddedObserver new_tab_observer;
#if defined(OS_MACOSX)
      const char* new_tab_click_script_template =
          "simulateClick(\"%s\", { metaKey: true });";
#else
      const char* new_tab_click_script_template =
          "simulateClick(\"%s\", { ctrlKey: true });";
#endif
      std::string new_tab_click_script = base::StringPrintf(
          new_tab_click_script_template, id_of_anchor_to_click);
      EXPECT_TRUE(ExecuteScript(main_contents, new_tab_click_script));

      new_contents = new_tab_observer.GetWebContents();
    }

    EXPECT_TRUE(WaitForLoadStop(new_contents));
    EXPECT_LT(1, browser()->tab_strip_model()->count());   
    CHECK_NE(TabStripModel::kNoTab,
             browser()->tab_strip_model()->GetIndexOfWebContents(new_contents));
    GURL expected_url(embedded_test_server()->GetURL("/title1.html"));
    EXPECT_EQ(expected_url, new_contents->GetLastCommittedURL());

    VerifyProcessExpectations(main_contents, new_contents);

    {
      std::string name_of_main_contents_window;
      EXPECT_TRUE(ExecuteScriptAndExtractString(
          main_contents, "window.domAutomationController.send(window.name)",
          &name_of_main_contents_window));
      EXPECT_EQ("main_contents", name_of_main_contents_window);

      bool window_opener_cast_to_bool = true;
      EXPECT_TRUE(ExecuteScriptAndExtractBool(
          new_contents, "window.domAutomationController.send(!!window.opener)",
          &window_opener_cast_to_bool));
      EXPECT_FALSE(window_opener_cast_to_bool);

      VerifyBrowsingInstanceExpectations(main_contents, new_contents);
    }

    return new_contents;
  }
