  std::string PerformCopy() {
    clipboard_message_watcher_->InitWait();
    RenderFrameHostImpl* rfh =
        static_cast<RenderFrameHostImpl*>(interstitial_->GetMainFrame());
    rfh->GetRenderWidgetHost()->delegate()->Copy();
    clipboard_message_watcher_->WaitForWriteCommit();
    return clipboard_message_watcher_->last_text();
  }
