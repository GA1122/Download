  ExtensionViewAccumulator(const std::string& extension_id,
                           int browser_window_id,
                           ViewType::Type view_type)
      : extension_id_(extension_id),
        browser_window_id_(browser_window_id),
        view_type_(view_type),
        views_(v8::Array::New()),
        index_(0) {
  }
