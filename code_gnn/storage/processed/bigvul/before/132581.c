void WebKitTestController::OnLoadURLForFrame(const GURL& url,
                                             const std::string& frame_name) {
  main_window_->LoadURLForFrame(url, frame_name);
}
