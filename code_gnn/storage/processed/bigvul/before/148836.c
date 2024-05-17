  void PerformSelectAll() {
    const base::string16 expected_title =
        base::UTF8ToUTF16("SELECTION_CHANGED");
    content::TitleWatcher title_watcher(shell()->web_contents(),
                                        expected_title);
    RenderFrameHostImpl* rfh =
        static_cast<RenderFrameHostImpl*>(interstitial_->GetMainFrame());
    rfh->GetRenderWidgetHost()->delegate()->SelectAll();
    EXPECT_EQ(expected_title, title_watcher.WaitAndGetTitle());
  }
