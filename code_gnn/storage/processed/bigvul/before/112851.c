  void VerifyPrintPreviewCancelled(bool did_cancel) {
    bool print_preview_cancelled =
        (render_thread_->sink().GetUniqueMessageMatching(
            PrintHostMsg_PrintPreviewCancelled::ID) != NULL);
    EXPECT_EQ(did_cancel, print_preview_cancelled);
  }
