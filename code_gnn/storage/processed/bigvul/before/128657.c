  std::string getBasePageUrlFromRequest() {
    std::string result;
    discourse_context::ClientDiscourseContext cdc =
        GetDiscourseContextFromRequest();
    if (cdc.display_size() > 0) {
      const discourse_context::Display& first_display = cdc.display(0);
      result = first_display.uri();
    }
    return result;
  }
