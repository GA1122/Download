void WebKitTestController::DiscardMainWindow() {
  WebContentsObserver::Observe(NULL);
  if (test_phase_ != BETWEEN_TESTS) {
    Shell::CloseAllWindows();
    base::MessageLoop::current()->PostTask(FROM_HERE,
                                           base::MessageLoop::QuitClosure());
    test_phase_ = CLEAN_UP;
  } else if (main_window_) {
    main_window_->Close();
  }
  main_window_ = NULL;
  current_pid_ = base::kNullProcessId;
}
