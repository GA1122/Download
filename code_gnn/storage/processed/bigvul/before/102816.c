  static bool GetTopFramePath(const net::TestServer& http_server,
                              const net::TestServer& good_https_server,
                              const net::TestServer& bad_https_server,
                              std::string* top_frame_path) {
    GURL http_url = http_server.GetURL("files/ssl/google.html");
    GURL good_https_url = good_https_server.GetURL("files/ssl/google.html");
    GURL bad_https_url = bad_https_server.GetURL(
        "files/ssl/bad_iframe.html");

    std::vector<net::TestServer::StringPair> replacement_text_frame_left;
    replacement_text_frame_left.push_back(
        make_pair("REPLACE_WITH_HTTP_PAGE", http_url.spec()));
    replacement_text_frame_left.push_back(
        make_pair("REPLACE_WITH_GOOD_HTTPS_PAGE", good_https_url.spec()));
    replacement_text_frame_left.push_back(
        make_pair("REPLACE_WITH_BAD_HTTPS_PAGE", bad_https_url.spec()));
    std::string frame_left_path;
    if (!net::TestServer::GetFilePathWithReplacements(
            "frame_left.html",
            replacement_text_frame_left,
            &frame_left_path))
      return false;

    std::vector<net::TestServer::StringPair> replacement_text_top_frame;
    replacement_text_top_frame.push_back(
        make_pair("REPLACE_WITH_FRAME_LEFT_PATH", frame_left_path));
    return net::TestServer::GetFilePathWithReplacements(
        "files/ssl/top_frame.html",
        replacement_text_top_frame,
        top_frame_path);
  }
