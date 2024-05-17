  void VerifyPrintPreviewGenerated(bool generated_preview) {
    const IPC::Message* preview_msg =
        render_thread_->sink().GetUniqueMessageMatching(
            PrintHostMsg_MetafileReadyForPrinting::ID);
    bool did_get_preview_msg = (NULL != preview_msg);
    ASSERT_EQ(generated_preview, did_get_preview_msg);
    if (did_get_preview_msg) {
      PrintHostMsg_MetafileReadyForPrinting::Param preview_param;
      PrintHostMsg_MetafileReadyForPrinting::Read(preview_msg, &preview_param);
      EXPECT_NE(0, preview_param.a.document_cookie);
      EXPECT_NE(0, preview_param.a.expected_pages_count);
      EXPECT_NE(0U, preview_param.a.data_size);
    }
  }
