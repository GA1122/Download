  static bool GetPageWithUnsafeWorkerPath(
      const net::TestServer& expired_https_server,
      std::string* page_with_unsafe_worker_path) {
    GURL imported_js_url = expired_https_server.GetURL("files/ssl/imported.js");
    std::vector<net::TestServer::StringPair> replacement_text_for_unsafe_worker;
    replacement_text_for_unsafe_worker.push_back(
        make_pair("REPLACE_WITH_IMPORTED_JS_URL", imported_js_url.spec()));
    std::string unsafe_worker_path;
    if (!net::TestServer::GetFilePathWithReplacements(
        "unsafe_worker.js",
        replacement_text_for_unsafe_worker,
        &unsafe_worker_path))
      return false;

    std::vector<net::TestServer::StringPair>
        replacement_text_for_page_with_unsafe_worker;
    replacement_text_for_page_with_unsafe_worker.push_back(
        make_pair("REPLACE_WITH_UNSAFE_WORKER_PATH", unsafe_worker_path));
    return net::TestServer::GetFilePathWithReplacements(
        "files/ssl/page_with_unsafe_worker.html",
        replacement_text_for_page_with_unsafe_worker,
        page_with_unsafe_worker_path);
  }
