  PrerenderBrowserTest()
      : prerender_contents_factory_(NULL),
        use_https_src_server_(false),
        call_javascript_(true) {
    EnableDOMAutomation();
  }
