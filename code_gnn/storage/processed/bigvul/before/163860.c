  bool WARN_UNUSED_RESULT HasCookie(WebContents* contents,
                                    const std::string& cookie) {
    int value_size;
    std::string actual_cookie;
    ui_test_utils::GetCookies(contents->GetURL(), contents, &value_size,
                              &actual_cookie);
    return actual_cookie.find(cookie) != std::string::npos;
  }
