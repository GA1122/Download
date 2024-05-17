  void VerifyDidPreviewPage(bool generate_draft_pages, int page_number) {
    bool msg_found = false;
    size_t msg_count = render_thread_->sink().message_count();
    for (size_t i = 0; i < msg_count; ++i) {
      const IPC::Message* msg = render_thread_->sink().GetMessageAt(i);
      if (msg->type() == PrintHostMsg_DidPreviewPage::ID) {
        PrintHostMsg_DidPreviewPage::Param page_param;
        PrintHostMsg_DidPreviewPage::Read(msg, &page_param);
        if (page_param.a.page_number == page_number) {
          msg_found = true;
          if (generate_draft_pages)
            EXPECT_NE(0U, page_param.a.data_size);
          else
            EXPECT_EQ(0U, page_param.a.data_size);
          break;
        }
      }
    }
    ASSERT_EQ(generate_draft_pages, msg_found);
  }
