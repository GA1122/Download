void WebKitTestController::OnShowDevTools(const std::string& settings,
                                          const std::string& frontend_url) {
  if (!devtools_frontend_) {
    devtools_frontend_ = LayoutTestDevToolsFrontend::Show(
        main_window_->web_contents(), settings, frontend_url);
  } else {
    devtools_frontend_->ReuseFrontend(settings, frontend_url);
  }
  devtools_frontend_->Activate();
  devtools_frontend_->Focus();
}
