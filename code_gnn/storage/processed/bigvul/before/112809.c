 ChromeMockRenderThread::ChromeMockRenderThread()
     : printer_(new MockPrinter),
       print_dialog_user_response_(true),
      print_preview_cancel_page_number_(-1),
      print_preview_pages_remaining_(0) {
}
