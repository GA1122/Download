  void MonitorResourceRequest(const net::test_server::HttpRequest& request) {
    GURL requested_url = request.GetURL();
    auto it = request.headers.find("Host");
    if (it != request.headers.end())
      requested_url = GURL("http://" + it->second + request.relative_url);

    if (!saw_request_url_ && request_url_ == requested_url) {
      saw_request_url_ = true;
      request_content_ = request.content;
      if (run_loop_)
        run_loop_->Quit();
    }
  }
