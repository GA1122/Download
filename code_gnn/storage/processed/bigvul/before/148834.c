  std::string PerformCut() {
    clipboard_message_watcher_->InitWait();
    const base::string16 expected_title = base::UTF8ToUTF16("TEXT_CHANGED");
    content::TitleWatcher title_watcher(shell()->web_contents(),
                                        expected_title);
    RenderFrameHostImpl* rfh =
        static_cast<RenderFrameHostImpl*>(interstitial_->GetMainFrame());
    rfh->GetRenderWidgetHost()->delegate()->Cut();
    clipboard_message_watcher_->WaitForWriteCommit();
    EXPECT_EQ(expected_title, title_watcher.WaitAndGetTitle());
    return clipboard_message_watcher_->last_text();
  }
