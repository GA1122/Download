  void VerifyPrintPreviewFailed(bool did_fail) {
    bool print_preview_failed =
        (render_thread_->sink().GetUniqueMessageMatching(
            PrintHostMsg_PrintPreviewFailed::ID) != NULL);
    EXPECT_EQ(did_fail, print_preview_failed);
  }
