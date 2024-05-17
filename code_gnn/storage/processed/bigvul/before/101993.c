  void VerifyPrintFailed(bool did_fail) {
    bool print_failed = (render_thread_.sink().GetUniqueMessageMatching(
        PrintHostMsg_PrintingFailed::ID) != NULL);
    EXPECT_EQ(did_fail, print_failed);
  }
