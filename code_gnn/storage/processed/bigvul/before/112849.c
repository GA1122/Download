  void VerifyPagesPrinted(bool printed) {
#if defined(OS_CHROMEOS)
    bool did_print_msg = (render_thread_->sink().GetUniqueMessageMatching(
        PrintHostMsg_TempFileForPrintingWritten::ID) != NULL);
    ASSERT_EQ(printed, did_print_msg);
#else
    const IPC::Message* print_msg =
        render_thread_->sink().GetUniqueMessageMatching(
            PrintHostMsg_DidPrintPage::ID);
    bool did_print_msg = (NULL != print_msg);
    ASSERT_EQ(printed, did_print_msg);
    if (printed) {
      PrintHostMsg_DidPrintPage::Param post_did_print_page_param;
      PrintHostMsg_DidPrintPage::Read(print_msg, &post_did_print_page_param);
      EXPECT_EQ(0, post_did_print_page_param.a.page_number);
    }
#endif   
  }
